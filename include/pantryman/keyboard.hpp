//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <cstdint>

namespace pm
{

    /// Input keys.
    enum class Key : uint16_t
    {
        UNKNOWN,

        // Letters
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

        // Numbers
        NUM_0,
        NUM_1,
        NUM_2,
        NUM_3,
        NUM_4,
        NUM_5,
        NUM_6,
        NUM_7,
        NUM_8,
        NUM_9,

        // Numpad
        NUMPAD_0,
        NUMPAD_1,
        NUMPAD_2,
        NUMPAD_3,
        NUMPAD_4,
        NUMPAD_5,
        NUMPAD_6,
        NUMPAD_7,
        NUMPAD_8,
        NUMPAD_9,
        NUMPAD_DECIMAL,
        NUMPAD_ENTER,
        NUMPAD_ADD,
        NUMPAD_SUBTRACT,
        NUMPAD_MULTIPLY,
        NUMPAD_DIVIDE,
        NUM_LOCK,

        // F buttons
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,

        // Symbols
        BACKTICK,
        DASH,
        EQUAL,
        LEFT_BRACKET,
        RIGHT_BRACKET,
        COLON,
        QUOTE,
        COMMA,
        PERIOD,
        SLASH,

        // Arrows
        UP,
        DOWN,
        LEFT,
        RIGHT,

        // Navigation
        INSERT,
        DELETE,
        HOME,
        END,
        PAGE_UP,
        PAGE_DOWN,
        BACKSPACE,

        // Misc controls
        ESCAPE,
        SPACE,
        ENTER,
        TAB,
        CAPS_LOCK,
        LEFT_SHIFT,
        RIGHT_SHIFT,
        LEFT_CONTROL,
        RIGHT_CONTROL,
        LEFT_ALT,
        RIGHT_ALT,
        LEFT_OS_LOGO,
        RIGHT_OS_LOGO,
    };

} // namespace pm
