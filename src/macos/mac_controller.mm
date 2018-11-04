//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/platform.hpp>

#if PM_OS_MACOS

#import <Foundation/Foundation.h>
#import <IOKit/hid/IOHIDLIB.h>

#include <pantryman/controller.hpp>
#include <pantryman/error.hpp>
#include "../controller.hpp"
#include "mac_controller.hpp"
#include "mac_util.hpp"

#include <mutex>

namespace pm
{

    namespace
    {

        class Controller
        {
        public:

            Controller()
            {
                std::memset(&m_state, 0, sizeof(m_state));
            }

            ~Controller()
            {
                disconnect();
            }

            void connect(IOHIDDeviceRef device)
            {
                NSNumber* vidValue = PM_BRIDGE(NSNumber*, IOHIDDeviceGetProperty(device, CFSTR(kIOHIDVendorIDKey)));
                NSNumber* pidValue = PM_BRIDGE(NSNumber*, IOHIDDeviceGetProperty(device, CFSTR(kIOHIDProductIDKey)));
                const uint16_t vid = [vidValue unsignedShortValue];
                const uint16_t pid = [pidValue unsignedShortValue];

                switch (vid)
                {
                    case 0x045e:            // Microsoft
                        connectXbox(device, pid);
                        break;

                    default:
                        break;
                }

                if (m_device)
                {
                    IOHIDDeviceRegisterInputValueCallback(m_device, &Controller::inputCallback, this);
                }
            }

            void disconnect()
            {
                if (m_device)
                {
                    CFRelease(m_device);
                    m_device = nullptr;
                }

                std::memset(&m_state, 0, sizeof(m_state));
            }

            bool isConnected() const
            {
                return m_device != nullptr;
            }

            IOHIDDeviceRef device() const
            {
                return m_device;
            }

            const ControllerState& getState() const
            {
                return m_state;
            }

        private:


            void connectXbox(IOHIDDeviceRef device, uint16_t pid)
            {
                switch (pid)
                {
                    case 0x028e:    // Xbox 360 Controller
                    case 0x028f:    // Xbox 360 Wireless Controller
                    case 0x02d1:    // Xbox One Controller
                    case 0x02dd:    // Xbox One Controller (firmware 2015)
                    case 0x02e3:    // Xbox One Elite Controller
                    case 0x02ea:    // Xbox One S Controller
                    case 0x02fd:    // Xbox One S Controller [Bluetooth]
                        break;

                    default:
                        return;
                }

                m_state.type = ControllerType::XBOX;
                m_device     = (IOHIDDeviceRef)CFRetain(device);

                NSArray* elements = PM_AUTORELEASE(PM_BRIDGE_TRANSFER(NSArray*, IOHIDDeviceCopyMatchingElements(m_device, nullptr, 0)));

                for (id elem in elements)
                {
                    IOHIDElementRef element = PM_BRIDGE(IOHIDElementRef, elem);

                    const uint32_t usagePage = IOHIDElementGetUsagePage(element);
                    const uint32_t usage     = IOHIDElementGetUsage(element);
                    NSValue*       value     = nil;

                    switch (usagePage)
                    {
                        case kHIDPage_GenericDesktop:
                            switch (usage)
                            {
                                case kHIDUsage_GD_X:
                                    remapAxis(element, 0, UINT16_MAX, INT16_MIN, INT16_MAX);
                                    value = [NSNumber numberWithInt:ElementType::LEFT_STICK_X];
                                    break;

                                case kHIDUsage_GD_Y:
                                    remapAxis(element, 0, UINT16_MAX, INT16_MIN, INT16_MAX);
                                    value = [NSNumber numberWithInt:ElementType::LEFT_STICK_Y];
                                    break;

                                case kHIDUsage_GD_Z:
                                    remapAxis(element, 0, UINT16_MAX, INT16_MIN, INT16_MAX);
                                    value = [NSNumber numberWithInt:ElementType::RIGHT_STICK_X];
                                    break;

                                case kHIDUsage_GD_Rz:
                                    remapAxis(element, 0, UINT16_MAX, INT16_MIN, INT16_MAX);
                                    value = [NSNumber numberWithInt:ElementType::RIGHT_STICK_Y];
                                    break;

                                case kHIDUsage_GD_Hatswitch:
                                    value = [NSNumber numberWithInt:ElementType::DPAD_HAT];
                                    break;
                            }
                            break;

                        case kHIDPage_Simulation:
                            switch (usage)
                            {
                                case kHIDUsage_Sim_Accelerator:
                                    remapAxis(element, 0, 1023, 0, UINT8_MAX);
                                    value = [NSNumber numberWithInt:ElementType::RIGHT_TRIGGER];
                                    break;

                                case kHIDUsage_Sim_Brake:
                                    remapAxis(element, 0, 1023, 0, UINT8_MAX);
                                    value = [NSNumber numberWithInt:ElementType::LEFT_TRIGGER];
                                    break;
                            }
                            break;

                        case kHIDPage_Button:
                            switch (usage)
                            {
                                case 0x1: value = [NSNumber numberWithInt:ElementType::BOTTOM_BUTTON];      break;
                                case 0x2: value = [NSNumber numberWithInt:ElementType::RIGHT_BUTTON];       break;
                                case 0x4: value = [NSNumber numberWithInt:ElementType::LEFT_BUTTON];        break;
                                case 0x5: value = [NSNumber numberWithInt:ElementType::TOP_BUTTON];         break;
                                case 0x7: value = [NSNumber numberWithInt:ElementType::LEFT_BUMPER];        break;
                                case 0x8: value = [NSNumber numberWithInt:ElementType::RIGHT_BUMPER];       break;
                                case 0xC: value = [NSNumber numberWithInt:ElementType::RIGHT_META];         break;
                                case 0xE: value = [NSNumber numberWithInt:ElementType::LEFT_STICK_BUTTON];  break;
                                case 0xF: value = [NSNumber numberWithInt:ElementType::RIGHT_STICK_BUTTON]; break;
                                default: break;
                            }
                            break;

                        case kHIDPage_Consumer:
                            if (usage == kHIDUsage_Csmr_ACBack)
                            {
                                value = [NSNumber numberWithInt:ElementType::LEFT_META];
                            }
                            break;
                    }

                    if (value)
                    {
                        IOHIDElementSetProperty(element, PM_BRIDGE(CFStringRef, @"pmEt"), PM_BRIDGE(CFTypeRef, value));
                        PM_RELEASE(value);
                        value = nil;
                    }
                }
            }

            void remapAxis(IOHIDElementRef element, CFIndex minInput, CFIndex maxInput, CFIndex minOutput, CFIndex maxOutput)
            {
                if (!element)
                {
                    return;
                }

                IOHIDElementSetProperty(element, CFSTR(kIOHIDElementCalibrationMinKey), PM_BRIDGE(CFNumberRef, @(minOutput)));
                IOHIDElementSetProperty(element, CFSTR(kIOHIDElementCalibrationMaxKey), PM_BRIDGE(CFNumberRef, @(maxOutput)));
                IOHIDElementSetProperty(element, CFSTR(kIOHIDElementCalibrationSaturationMinKey), PM_BRIDGE(CFNumberRef, @(minInput)));
                IOHIDElementSetProperty(element, CFSTR(kIOHIDElementCalibrationSaturationMaxKey), PM_BRIDGE(CFNumberRef, @(maxInput)));
            }

            static int16_t flipAxis(int16_t axis)
            {
                return int16_t(-int32_t(axis) - 1);
            }

            static void updateDpadState(ControllerButton* buttons, IOHIDValueRef value)
            {
                constexpr uint32_t DPAD_MASK = 0
                    | uint32_t(ControllerButton::DPAD_LEFT)
                    | uint32_t(ControllerButton::DPAD_UP)
                    | uint32_t(ControllerButton::DPAD_RIGHT)
                    | uint32_t(ControllerButton::DPAD_DOWN);

                *buttons = *buttons & ControllerButton(~DPAD_MASK);

                if (const CFIndex state = IOHIDValueGetIntegerValue(value))
                {
                    updateButtonState(buttons, (state >= 6),               ControllerButton::DPAD_LEFT);
                    updateButtonState(buttons, (state == 8 || state <= 2), ControllerButton::DPAD_UP);
                    updateButtonState(buttons, (state >= 2 && state <= 4), ControllerButton::DPAD_RIGHT);
                    updateButtonState(buttons, (state >= 4 && state <= 6), ControllerButton::DPAD_DOWN);
                }
            }

            static void updateButtonState(ControllerButton* buttons, bool value, ControllerButton button)
            {
                if (value)
                {
                    *buttons = *buttons | button;
                }
                else
                {
                    *buttons = *buttons & ControllerButton(~uint32_t(button));
                }
            }

            static void updateButtonState(ControllerButton* buttons, IOHIDValueRef value, ControllerButton button)
            {
                updateButtonState(buttons, !!IOHIDValueGetIntegerValue(value), button);
            }

            static int16_t getAxisState(IOHIDValueRef value)
            {
                const float axisValue = IOHIDValueGetScaledValue(value, kIOHIDValueScaleTypeCalibrated);
                return int16_t(axisValue);
            }

            static void inputCallback(void* context, IOReturn result, void* sender, IOHIDValueRef value)
            {
                if (result != kIOReturnSuccess)
                {
                    return;
                }

                Controller*     controller = static_cast<Controller*>(context);
                IOHIDElementRef element    = IOHIDValueGetElement(value);
                NSNumber*       elemType   = PM_BRIDGE(NSNumber*, IOHIDElementGetProperty(element, PM_BRIDGE(CFStringRef, @"pmEt")));

                switch (ElementType::Type([elemType intValue]))
                {
                    // Axes
                    case ElementType::LEFT_STICK_X:  controller->m_state.leftStickX  = getAxisState(value);           break;
                    case ElementType::LEFT_STICK_Y:  controller->m_state.leftStickY  = flipAxis(getAxisState(value)); break;
                    case ElementType::RIGHT_STICK_X: controller->m_state.rightStickX = getAxisState(value);           break;
                    case ElementType::RIGHT_STICK_Y: controller->m_state.rightStickY = flipAxis(getAxisState(value)); break;

                    case ElementType::LEFT_TRIGGER:
                        controller->m_state.leftTrigger = getAxisState(value);
                        updateButtonState(&controller->m_state.buttons, controller->m_state.leftTrigger, ControllerButton::LEFT_TRIGGER);
                        break;

                    case ElementType::RIGHT_TRIGGER:
                        controller->m_state.rightTrigger = getAxisState(value);
                        updateButtonState(&controller->m_state.buttons, controller->m_state.rightTrigger, ControllerButton::RIGHT_TRIGGER);
                        break;

                    case ElementType::DPAD_HAT:
                        updateDpadState(&controller->m_state.buttons, value);
                        break;

                    // Buttons
                    case ElementType::LEFT_BUTTON:        updateButtonState(&controller->m_state.buttons, value, ControllerButton::LEFT_BUTTON);   break;
                    case ElementType::TOP_BUTTON:         updateButtonState(&controller->m_state.buttons, value, ControllerButton::TOP_BUTTON);    break;
                    case ElementType::RIGHT_BUTTON:       updateButtonState(&controller->m_state.buttons, value, ControllerButton::RIGHT_BUTTON);  break;
                    case ElementType::BOTTOM_BUTTON:      updateButtonState(&controller->m_state.buttons, value, ControllerButton::BOTTOM_BUTTON); break;
                    case ElementType::LEFT_STICK_BUTTON:  updateButtonState(&controller->m_state.buttons, value, ControllerButton::LEFT_STICK);    break;
                    case ElementType::RIGHT_STICK_BUTTON: updateButtonState(&controller->m_state.buttons, value, ControllerButton::RIGHT_STICK);   break;
                    case ElementType::LEFT_BUMPER:        updateButtonState(&controller->m_state.buttons, value, ControllerButton::LEFT_BUMPER);   break;
                    case ElementType::RIGHT_BUMPER:       updateButtonState(&controller->m_state.buttons, value, ControllerButton::RIGHT_BUMPER);  break;
                    case ElementType::LEFT_META:          updateButtonState(&controller->m_state.buttons, value, ControllerButton::LEFT_META);     break;
                    case ElementType::RIGHT_META:         updateButtonState(&controller->m_state.buttons, value, ControllerButton::RIGHT_META);    break;

                    default:
                        break;
                }
            }

            struct ElementType
            {
                enum Type : int
                {
                    // Axes
                    LEFT_STICK_X,
                    LEFT_STICK_Y,
                    RIGHT_STICK_X,
                    RIGHT_STICK_Y,
                    LEFT_TRIGGER,
                    RIGHT_TRIGGER,
                    DPAD_HAT,

                    // Buttons
                    LEFT_BUTTON,
                    TOP_BUTTON,
                    RIGHT_BUTTON,
                    BOTTOM_BUTTON,
                    LEFT_STICK_BUTTON,
                    RIGHT_STICK_BUTTON,
                    LEFT_BUMPER,
                    RIGHT_BUMPER,
                    LEFT_META,
                    RIGHT_META,
                };
            };

            ControllerState m_state;
            IOHIDDeviceRef  m_device{nullptr};

        };

        class ControllerMgr
        {
        public:

            ControllerMgr()
            {
                m_hid = IOHIDManagerCreate(kCFAllocatorDefault, 0);

                memset(m_connected, 0, sizeof(m_connected));
                memset(m_states, 0, sizeof(m_states));
            }

            ~ControllerMgr()
            {
                CFRelease(m_hid);
                m_hid = nullptr;
            }

            void init()
            {
                if (IOHIDManagerOpen(m_hid, kIOHIDOptionsTypeNone) != kIOReturnSuccess)
                {
                    // Issue #1: Add callback for when controller initialization fails.
                    return;
                }

                NSArray* matching = @[
                    @{@(kIOHIDDeviceUsagePageKey): @(kHIDPage_GenericDesktop), @(kIOHIDDeviceUsageKey): @(kHIDUsage_GD_GamePad)},
                    @{@(kIOHIDDeviceUsagePageKey): @(kHIDPage_GenericDesktop), @(kIOHIDDeviceUsageKey): @(kHIDUsage_GD_MultiAxisController)},
                ];

                IOHIDManagerRegisterDeviceMatchingCallback(m_hid, &ControllerMgr::deviceMatched, this);
                IOHIDManagerRegisterDeviceRemovalCallback(m_hid, &ControllerMgr::deviceRemoved, this);
                IOHIDManagerSetDeviceMatchingMultiple(m_hid, PM_BRIDGE(CFArrayRef, matching));
                IOHIDManagerScheduleWithRunLoop(m_hid, CFRunLoopGetMain(), kCFRunLoopDefaultMode);
            }

            void destroy()
            {
                IOHIDManagerClose(m_hid, 0);
            }

            void update()
            {
                std::lock_guard<std::mutex> lock{m_mutex};

                bool*            connected = m_connected;
                ControllerState* state     = m_states;

                for (Controller& controller : m_controllers)
                {
                    *(connected++) = controller.isConnected();
                    *(state++)     = controller.getState();
                }
            }

            void getState(uint8_t index, ControllerState* o_state, Error* o_err)
            {
                PM_ERROR_RETURN(o_err);
                assert(index <= MAX_CONTROLLERS);

                std::lock_guard<std::mutex> lock{m_mutex};

                if (!m_connected[index])
                {
                    PM_ERROR_UPDATE(o_err, ERR_NOT_CONNECTED);
                    return;
                }

                *o_state = m_states[index];
            }

        private:

            static void deviceMatched(void* context, IOReturn result, void* sender, IOHIDDeviceRef device)
            {
                if (result != kIOReturnSuccess)
                {
                    return;
                }

                // Make sure we (somehow) don't already know about this controller, and
                // find the first slot not in use.
                ControllerMgr* mgr         = static_cast<ControllerMgr*>(context);
                Controller*    firstUnused = nullptr;

                for (Controller& controller : mgr->m_controllers)
                {
                    if (controller.device() == device)
                    {
                        return;
                    }

                    if (!firstUnused && !controller.isConnected())
                    {
                        firstUnused = &controller;
                    }
                }

                if (!firstUnused)
                {
                    // All available slots are in use, so we can't connect.
                    return;
                }

                // Set up the controller.
                firstUnused->connect(device);
            }

            static void deviceRemoved(void* context, IOReturn result, void* sender, IOHIDDeviceRef device)
            {
                if (result != kIOReturnSuccess)
                {
                    return;
                }

                ControllerMgr* mgr = static_cast<ControllerMgr*>(context);

                for (Controller& controller : mgr->m_controllers)
                {
                    if (controller.device() == device)
                    {
                        controller.disconnect();
                        return;
                    }
                }
            }

            Controller      m_controllers[MAX_CONTROLLERS];
            IOHIDManagerRef m_hid;

            std::mutex      m_mutex;
            bool            m_connected[MAX_CONTROLLERS];
            ControllerState m_states[MAX_CONTROLLERS];

        };

        static ControllerMgr s_mgr;

    } // namespace

    void controllerInit()
    {
        s_mgr.init();
    }

    void controllerUpdate()
    {
        s_mgr.update();
    }

    void controllerDestroy()
    {
        s_mgr.destroy();
    }

    void getState(uint8_t controller, ControllerState* o_state, Error* o_err)
    {
        s_mgr.getState(controller, o_state, o_err);
    }

} // namespace pm

#endif // PM_OS_MACOS
