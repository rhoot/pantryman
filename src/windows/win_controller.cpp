//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/platform.hpp>

#if PM_OS_WINDOWS

#include <pantryman/controller.hpp>
#include "../compat/windows.hpp"
#include "../controller.hpp"

// Only used for the declarations, we'll import the necessary functions
// manually to support all XInput versions.
#include <Xinput.h>

namespace pm
{

    static decltype(XInputGetState)* s_getState;
    static bool                      s_loaded;
    static HMODULE                   s_module;

    static void loadXinput()
    {
        if (s_loaded)
        {
            return;
        }

        // Try to load (in order): XInput 1.4, 9.1.0, 1.3, 1.1
        if ((s_module = LoadLibraryA("xinput1_4")) != nullptr
            || (s_module = LoadLibraryA("xinput9_1_0")) != nullptr
            || (s_module = LoadLibraryA("xinput1_3")) != nullptr
            || (s_module = LoadLibraryA("xinput1_1")) != nullptr
        ) {
            s_getState = (decltype(s_getState))GetProcAddress(s_module, "XInputGetState");
        }

        s_loaded = true;
    }

    static ControllerButton remap(WORD buttons, uint8_t index, ControllerButton button)
    {
        return ControllerButton(((buttons >> index) & 1) * uint16_t(button));
    }

    void getState(uint8_t controller, ControllerState* o_state, Error* o_err)
    {
        PM_ERROR_RETURN(o_err);

        loadXinput();

        if (!s_getState)
        {
            PM_ERROR_UPDATE(o_err, ERR_FAILED);
            return;
        }

        if (controller >= MAX_CONTROLLERS)
        {
            PM_ERROR_UPDATE(o_err, ERR_INVALID_CONTROLLER);
            return;
        }

        XINPUT_STATE state;
        DWORD status = s_getState(controller, &state);

        switch (status)
        {
            case ERROR_SUCCESS:
                o_state->buttons = ControllerButton::NONE
                    | (state.Gamepad.bLeftTrigger  ? ControllerButton::LEFT_TRIGGER  : ControllerButton::NONE)
                    | (state.Gamepad.bRightTrigger ? ControllerButton::RIGHT_TRIGGER : ControllerButton::NONE)
                    | remap(state.Gamepad.wButtons,  0, ControllerButton::DPAD_UP)
                    | remap(state.Gamepad.wButtons,  1, ControllerButton::DPAD_DOWN)
                    | remap(state.Gamepad.wButtons,  2, ControllerButton::DPAD_LEFT)
                    | remap(state.Gamepad.wButtons,  3, ControllerButton::DPAD_RIGHT)
                    | remap(state.Gamepad.wButtons,  4, ControllerButton::XBOX_START)
                    | remap(state.Gamepad.wButtons,  5, ControllerButton::XBOX_BACK)
                    | remap(state.Gamepad.wButtons,  6, ControllerButton::LEFT_STICK)
                    | remap(state.Gamepad.wButtons,  7, ControllerButton::RIGHT_STICK)
                    | remap(state.Gamepad.wButtons,  8, ControllerButton::LEFT_BUMPER)
                    | remap(state.Gamepad.wButtons,  9, ControllerButton::RIGHT_BUMPER)
                    | remap(state.Gamepad.wButtons, 12, ControllerButton::XBOX_A)
                    | remap(state.Gamepad.wButtons, 13, ControllerButton::XBOX_B)
                    | remap(state.Gamepad.wButtons, 14, ControllerButton::XBOX_X)
                    | remap(state.Gamepad.wButtons, 15, ControllerButton::XBOX_Y);

                o_state->leftStickX     = state.Gamepad.sThumbLX;
                o_state->leftStickY     = state.Gamepad.sThumbLY;
                o_state->rightStickX    = state.Gamepad.sThumbRX;
                o_state->rightStickY    = state.Gamepad.sThumbRY;
                o_state->leftTrigger    = state.Gamepad.bLeftTrigger;
                o_state->rightTrigger   = state.Gamepad.bRightTrigger;
                o_state->type           = ControllerType::XBOX;
                break;

            case ERROR_DEVICE_NOT_CONNECTED:
                PM_ERROR_UPDATE(o_err, ERR_NOT_CONNECTED);
                break;

            default:
                PM_ERROR_UPDATE(o_err, ERR_FAILED);
                break;
        }
    }

} // namespace pm

#endif // PM_OS_WINDOWS
