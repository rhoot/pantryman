//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/platform.hpp>

#if PM_OS_MACOS

#import <GameController/GameController.h>
#import <IOKit/hid/IOHIDLIB.h>

#include <pantryman/controller.hpp>
#include <pantryman/error.hpp>
#include "../controller.hpp"
#include "mac_controller.hpp"
#include "mac_util.hpp"

#include <atomic>
#include <iterator>
#include <mutex>

namespace pm
{

    namespace
    {

        class Controller
        {
        public:

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
            }

            void disconnect()
            {
                if (m_elements)
                {
                    PM_RELEASE(m_elements);
                    m_elements = nil;
                }

                if (m_device)
                {
                    CFRelease(m_device);
                    m_device = nullptr;
                }

                m_type            = ControllerType::UNKNOWN;
                m_leftStickX      = nullptr;
                m_leftStickY      = nullptr;
                m_rightStickX     = nullptr;
                m_rightStickY     = nullptr;
                m_leftTrigger     = nullptr;
                m_rightTrigger    = nullptr;
                m_dpadHat         = nullptr;
                m_leftButton      = nullptr;
                m_topButton       = nullptr;
                m_rightButton     = nullptr;
                m_bottomButton    = nullptr;
                m_leftStickBtn    = nullptr;
                m_rightStickBtn   = nullptr;
                m_leftBumper      = nullptr;
                m_rightBumper     = nullptr;
                m_leftMeta        = nullptr;
                m_rightMeta       = nullptr;
            }

            bool isConnected() const
            {
                return m_device != nullptr;
            }

            IOHIDDeviceRef device() const
            {
                return m_device;
            }

            bool getState(ControllerState* o_state)
            {
                if (!m_device)
                {
                    return false;
                }

                o_state->type        = m_type;
                o_state->leftStickX  = getAxisState(m_leftStickX);
                o_state->leftStickY  = flipAxis(getAxisState(m_leftStickY));
                o_state->rightStickX = getAxisState(m_rightStickX);
                o_state->rightStickY = flipAxis(getAxisState(m_rightStickY));
                o_state->leftTrigger = uint8_t(getAxisState(m_leftTrigger));
                o_state->rightTrigger = uint8_t(getAxisState(m_rightTrigger));

                o_state->buttons = ControllerButton::NONE
                    | getDpadState()
                    | (o_state->leftTrigger  ? ControllerButton::LEFT_TRIGGER  : ControllerButton::NONE)
                    | (o_state->rightTrigger ? ControllerButton::RIGHT_TRIGGER : ControllerButton::NONE)
                    | getButtonState(m_leftButton,      ControllerButton::LEFT_BUTTON)
                    | getButtonState(m_topButton,       ControllerButton::TOP_BUTTON)
                    | getButtonState(m_rightButton,     ControllerButton::RIGHT_BUTTON)
                    | getButtonState(m_bottomButton,    ControllerButton::BOTTOM_BUTTON)
                    | getButtonState(m_leftStickBtn,    ControllerButton::LEFT_STICK)
                    | getButtonState(m_rightStickBtn,   ControllerButton::RIGHT_STICK)
                    | getButtonState(m_leftBumper,      ControllerButton::LEFT_BUMPER)
                    | getButtonState(m_rightBumper,     ControllerButton::RIGHT_BUMPER)
                    | getButtonState(m_leftMeta,        ControllerButton::LEFT_META)
                    | getButtonState(m_rightMeta,       ControllerButton::RIGHT_META);

                return true;
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

                m_type   = ControllerType::XBOX;
                m_device = (IOHIDDeviceRef)CFRetain(device);

                m_elements = PM_BRIDGE_TRANSFER(NSArray*, IOHIDDeviceCopyMatchingElements(m_device, nullptr, 0));

                for (id elem in m_elements)
                {
                    IOHIDElementRef element = PM_BRIDGE(IOHIDElementRef, elem);

                    const uint32_t usagePage = IOHIDElementGetUsagePage(element);
                    const uint32_t usage     = IOHIDElementGetUsage(element);

                    // These elements are retained by the m_elements array already.

                    switch (usagePage)
                    {
                        case kHIDPage_GenericDesktop:
                            switch (usage)
                            {
                                case kHIDUsage_GD_X:         m_leftStickX   = element; break;
                                case kHIDUsage_GD_Y:         m_leftStickY   = element; break;
                                case kHIDUsage_GD_Z:         m_rightStickX  = element; break;
                                case kHIDUsage_GD_Rz:        m_rightStickY  = element; break;
                                case kHIDUsage_GD_Hatswitch: m_dpadHat      = element; break;
                                default: break;
                            }
                            break;

                        case kHIDPage_Simulation:
                            switch (usage)
                            {
                                case kHIDUsage_Sim_Accelerator: m_rightTrigger = element; break;
                                case kHIDUsage_Sim_Brake:       m_leftTrigger  = element; break;
                                default: break;
                            }
                            break;

                        case kHIDPage_Button:
                            switch (usage)
                            {
                                case 0x1: m_bottomButton  = element; break;
                                case 0x2: m_rightButton   = element; break;
                                case 0x4: m_leftButton    = element; break;
                                case 0x5: m_topButton     = element; break;
                                case 0x7: m_leftBumper    = element; break;
                                case 0x8: m_rightBumper   = element; break;
                                case 0xC: m_rightMeta     = element; break;
                                case 0xE: m_leftStickBtn  = element; break;
                                case 0xF: m_rightStickBtn = element; break;
                                default: break;
                            }
                            break;

                        case kHIDPage_Consumer:
                            if (usage == kHIDUsage_Csmr_ACBack)
                            {
                                m_leftMeta = element;
                            }
                            break;

                        default:
                            break;
                    }
                }

                remapAxis(m_leftStickX, 0, UINT16_MAX, INT16_MIN, INT16_MAX);
                remapAxis(m_leftStickY, 0, UINT16_MAX, INT16_MIN, INT16_MAX);
                remapAxis(m_rightStickX, 0, UINT16_MAX, INT16_MIN, INT16_MAX);
                remapAxis(m_rightStickY, 0, UINT16_MAX, INT16_MIN, INT16_MAX);
                remapAxis(m_leftTrigger, 0, 1023, 0, UINT8_MAX);
                remapAxis(m_rightTrigger, 0, 1023, 0, UINT8_MAX);
            }

            void remapAxis(IOHIDElementRef element, CFIndex minInput, CFIndex maxInput, CFIndex minOutput, CFIndex maxOutput)
            {
                if (!element)
                {
                    return;
                }

                IOHIDElementSetProperty(element, CFSTR(kIOHIDElementCalibrationMinKey), PM_BRIDGE(CFTypeRef, @(minOutput)));
                IOHIDElementSetProperty(element, CFSTR(kIOHIDElementCalibrationMaxKey), PM_BRIDGE(CFTypeRef, @(maxOutput)));
                IOHIDElementSetProperty(element, CFSTR(kIOHIDElementCalibrationSaturationMinKey), PM_BRIDGE(CFTypeRef, @(minInput)));
                IOHIDElementSetProperty(element, CFSTR(kIOHIDElementCalibrationSaturationMaxKey), PM_BRIDGE(CFTypeRef, @(maxInput)));
            }

            int16_t flipAxis(int16_t axis)
            {
                return int16_t(-int32_t(axis) - 1);
            }

            ControllerButton getButtonState(IOHIDElementRef element, ControllerButton button)
            {
                IOHIDValueRef value;

                if (!element)
                {
                    return ControllerButton::NONE;
                }

                if (IOHIDDeviceGetValueWithOptions(m_device, element, &value, kIOHIDDeviceGetValueWithoutUpdate) != kIOReturnSuccess)
                {
                    return ControllerButton::NONE;
                }

                return IOHIDValueGetIntegerValue(value) ? button : ControllerButton::NONE;
            }

            ControllerButton getDpadState()
            {
                IOHIDValueRef value;

                if (!m_dpadHat)
                {
                    return ControllerButton::NONE;
                }

                if (IOHIDDeviceGetValueWithOptions(m_device, m_dpadHat, &value, kIOHIDDeviceGetValueWithoutUpdate) != kIOReturnSuccess)
                {
                    return ControllerButton::NONE;
                }

                const CFIndex state = IOHIDValueGetIntegerValue(value);

                if (!state)
                {
                    return ControllerButton::NONE;
                }

                return ControllerButton::NONE
                    | ((state >= 6)               ? ControllerButton::DPAD_LEFT  : ControllerButton::NONE)
                    | ((state == 8 || state <= 2) ? ControllerButton::DPAD_UP    : ControllerButton::NONE)
                    | ((state >= 2 && state <= 4) ? ControllerButton::DPAD_RIGHT : ControllerButton::NONE)
                    | ((state >= 4 && state <= 6) ? ControllerButton::DPAD_DOWN  : ControllerButton::NONE);
            }

            int16_t getAxisState(IOHIDElementRef element)
            {
                IOHIDValueRef value;

                if (!element)
                {
                    return 0;
                }

                if (IOHIDDeviceGetValueWithOptions(m_device, element, &value, kIOHIDDeviceGetValueWithoutUpdate) != kIOReturnSuccess)
                {
                    return 0;
                }

                float axisValue = IOHIDValueGetScaledValue(value, kIOHIDValueScaleTypeCalibrated);
                return int16_t(axisValue);
            }

            ControllerType  m_type{ControllerType::UNKNOWN};
            IOHIDDeviceRef  m_device{nullptr};
            NSArray*        m_elements{nullptr};

            // Axes
            IOHIDElementRef m_leftStickX{nullptr};
            IOHIDElementRef m_leftStickY{nullptr};
            IOHIDElementRef m_rightStickX{nullptr};
            IOHIDElementRef m_rightStickY{nullptr};
            IOHIDElementRef m_leftTrigger{nullptr};
            IOHIDElementRef m_rightTrigger{nullptr};
            IOHIDElementRef m_dpadHat{nullptr};

            // Buttons
            IOHIDElementRef m_leftButton{nullptr};
            IOHIDElementRef m_topButton{nullptr};
            IOHIDElementRef m_rightButton{nullptr};
            IOHIDElementRef m_bottomButton{nullptr};
            IOHIDElementRef m_leftStickBtn{nullptr};
            IOHIDElementRef m_rightStickBtn{nullptr};
            IOHIDElementRef m_leftBumper{nullptr};
            IOHIDElementRef m_rightBumper{nullptr};
            IOHIDElementRef m_leftMeta{nullptr};
            IOHIDElementRef m_rightMeta{nullptr};

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

//                NSArray* matching = @[
//                    @{@(kIOHIDDeviceUsagePageKey): @(kHIDPage_GenericDesktop), @(kIOHIDDeviceUsageKey): @(kHIDUsage_GD_GamePad)},
//                    @{@(kIOHIDDeviceUsagePageKey): @(kHIDPage_GenericDesktop), @(kIOHIDDeviceUsageKey): @(kHIDUsage_GD_MultiAxisController)},
//                ];

                IOHIDManagerRegisterDeviceMatchingCallback(m_hid, &ControllerMgr::deviceMatched, this);
                IOHIDManagerRegisterDeviceRemovalCallback(m_hid, &ControllerMgr::deviceRemoved, this);
                IOHIDManagerSetDeviceMatchingMultiple(m_hid, nullptr); //PM_BRIDGE(CFArrayRef, matching));
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
                    *(connected++) = controller.getState(state++);
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
