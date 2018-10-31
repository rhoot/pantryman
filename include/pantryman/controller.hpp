//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/error.hpp>

namespace pm
{

    /// Game controller buttons.
    enum class ControllerButton : uint32_t
    {
        NONE            = 0x00000000,

        // Generic buttons
        DPAD_LEFT       = 0x00000001,
        DPAD_UP         = 0x00000002,
        DPAD_RIGHT      = 0x00000004,
        DPAD_DOWN       = 0x00000008,
        LEFT_BUTTON     = 0x00000010,
        TOP_BUTTON      = 0x00000020,
        RIGHT_BUTTON    = 0x00000040,
        BOTTOM_BUTTON   = 0x00000080,
        LEFT_STICK      = 0x00000100,
        RIGHT_STICK     = 0x00000200,
        LEFT_BUMPER     = 0x00000400,
        RIGHT_BUMPER    = 0x00000800,
        LEFT_TRIGGER    = 0x00001000,
        RIGHT_TRIGGER   = 0x00002000,
        LEFT_META       = 0x00004000,
        RIGHT_META      = 0x00008000,

        // Xbox buttons
        XBOX_X          = LEFT_BUTTON,
        XBOX_Y          = TOP_BUTTON,
        XBOX_B          = RIGHT_BUTTON,
        XBOX_A          = BOTTOM_BUTTON,
        XBOX_BACK       = LEFT_META,
        XBOX_START      = RIGHT_META,
    };

    /// Game controller type.
    enum class ControllerType : uint8_t
    {
        UNKNOWN,
        XBOX,
    };

    /// Game controller state.
    struct ControllerState
    {
        /// Buttons that are currently pressed.
        ControllerButton buttons;

        /// Value of the X-axis of the left controller stick.
        int16_t leftStickX;

        /// Value of the Y-axis of the left controller stick.
        int16_t leftStickY;

        /// Value of the X-axis of the right controller stick.
        int16_t rightStickX;

        /// Value of the Y-axis of the right controller stick.
        int16_t rightStickY;

        /// Value of the left analog trigger value.
        uint8_t leftTrigger;

        /// Value of the right analog trigger value.
        uint8_t rightTrigger;

        /// Type of controller.
        ControllerType type;
    };

    /// Max amount of supported controllers.
    enum : uint8_t { MAX_CONTROLLERS = 4 };

    /// Error returned when a controller is not connected.
    extern Error ERR_NOT_CONNECTED;

    /// Error returned when an invalid controller index was provided.
    extern Error ERR_INVALID_CONTROLLER;

    /// Get the controller state for the given controller.
    void getState(uint8_t controller, ControllerState* o_state, Error* o_err);

    /// Test whether a given controller button is down.
    bool isDown(ControllerButton pressed, ControllerButton button);

    /// Test whether a given controller button is up.
    bool isUp(ControllerButton pressed, ControllerButton button);

    /// Convert the controller stick value to a normalized float between
    /// -1 and 1.
    float stickAxis(int16_t value);

    /// Convert the controller trigger value to a normalized float between
    /// 0 and 1.
    float triggerAxis(uint8_t value);

} // namespace pm
