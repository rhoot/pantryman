//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/platform.hpp>

#if PM_OS_MACOS

#import <AppKit/AppKit.h>
#import <Carbon/Carbon.h>

#include "mac_keyboard.hpp"
#include "../events.hpp"

namespace pm
{

    Key translateKey(uint16_t vk)
    {
        switch (vk)
        {
            case kVK_ANSI_A:
                return Key::A;
            case kVK_ANSI_B:
                return Key::B;
            case kVK_ANSI_C:
                return Key::C;
            case kVK_ANSI_D:
                return Key::D;
            case kVK_ANSI_E:
                return Key::E;
            case kVK_ANSI_F:
                return Key::F;
            case kVK_ANSI_G:
                return Key::G;
            case kVK_ANSI_H:
                return Key::H;
            case kVK_ANSI_I:
                return Key::I;
            case kVK_ANSI_J:
                return Key::J;
            case kVK_ANSI_K:
                return Key::K;
            case kVK_ANSI_L:
                return Key::L;
            case kVK_ANSI_M:
                return Key::M;
            case kVK_ANSI_N:
                return Key::N;
            case kVK_ANSI_O:
                return Key::O;
            case kVK_ANSI_P:
                return Key::P;
            case kVK_ANSI_Q:
                return Key::Q;
            case kVK_ANSI_R:
                return Key::R;
            case kVK_ANSI_S:
                return Key::S;
            case kVK_ANSI_T:
                return Key::T;
            case kVK_ANSI_U:
                return Key::U;
            case kVK_ANSI_V:
                return Key::V;
            case kVK_ANSI_W:
                return Key::W;
            case kVK_ANSI_X:
                return Key::X;
            case kVK_ANSI_Y:
                return Key::Y;
            case kVK_ANSI_Z:
                return Key::Z;
            case kVK_ANSI_0:
                return Key::NUM_0;
            case kVK_ANSI_1:
                return Key::NUM_1;
            case kVK_ANSI_2:
                return Key::NUM_2;
            case kVK_ANSI_3:
                return Key::NUM_3;
            case kVK_ANSI_4:
                return Key::NUM_4;
            case kVK_ANSI_5:
                return Key::NUM_5;
            case kVK_ANSI_6:
                return Key::NUM_6;
            case kVK_ANSI_7:
                return Key::NUM_7;
            case kVK_ANSI_8:
                return Key::NUM_8;
            case kVK_ANSI_9:
                return Key::NUM_9;
            case kVK_ANSI_Keypad0:
                return Key::NUMPAD_0;
            case kVK_ANSI_Keypad1:
                return Key::NUMPAD_1;
            case kVK_ANSI_Keypad2:
                return Key::NUMPAD_2;
            case kVK_ANSI_Keypad3:
                return Key::NUMPAD_3;
            case kVK_ANSI_Keypad4:
                return Key::NUMPAD_4;
            case kVK_ANSI_Keypad5:
                return Key::NUMPAD_5;
            case kVK_ANSI_Keypad6:
                return Key::NUMPAD_6;
            case kVK_ANSI_Keypad7:
                return Key::NUMPAD_7;
            case kVK_ANSI_Keypad8:
                return Key::NUMPAD_8;
            case kVK_ANSI_Keypad9:
                return Key::NUMPAD_9;
            case kVK_ANSI_KeypadDecimal:
                return Key::NUMPAD_DECIMAL;
            case kVK_ANSI_KeypadEnter:
                return Key::NUMPAD_ENTER;
            case kVK_ANSI_KeypadPlus:
                return Key::NUMPAD_ADD;
            case kVK_ANSI_KeypadMinus:
                return Key::NUMPAD_SUBTRACT;
            case kVK_ANSI_KeypadMultiply:
                return Key::NUMPAD_MULTIPLY;
            case kVK_ANSI_KeypadDivide:
                return Key::NUMPAD_DIVIDE;
            case kVK_ANSI_KeypadClear:
                return Key::NUM_LOCK;
            case kVK_Return:
                return Key::ENTER;
            case kVK_F1:
                return Key::F1;
            case kVK_F2:
                return Key::F2;
            case kVK_F3:
                return Key::F3;
            case kVK_F4:
                return Key::F4;
            case kVK_F5:
                return Key::F5;
            case kVK_F6:
                return Key::F6;
            case kVK_F7:
                return Key::F7;
            case kVK_F8:
                return Key::F8;
            case kVK_F9:
                return Key::F9;
            case kVK_F10:
                return Key::F10;
            case kVK_F11:
                return Key::F11;
            case kVK_F12:
                return Key::F12;
            case kVK_ANSI_Grave:
                return Key::BACKTICK;
            case kVK_ANSI_Minus:
                return Key::DASH;
            case kVK_ANSI_Equal:
                return Key::EQUAL;
            case kVK_ANSI_LeftBracket:
                return Key::LEFT_BRACKET;
            case kVK_ANSI_RightBracket:
                return Key::RIGHT_BRACKET;
            case kVK_ANSI_Semicolon:
                return Key::COLON;
            case kVK_ANSI_Quote:
                return Key::QUOTE;
            case kVK_ANSI_Comma:
                return Key::COMMA;
            case kVK_ANSI_Period:
                return Key::PERIOD;
            case kVK_ANSI_Slash:
                return Key::SLASH;
            case kVK_UpArrow:
                return Key::UP;
            case kVK_DownArrow:
                return Key::DOWN;
            case kVK_LeftArrow:
                return Key::LEFT;
            case kVK_RightArrow:
                return Key::RIGHT;
            case kVK_Help:
                return Key::INSERT;
            case kVK_ForwardDelete:
                return Key::DELETE;
            case kVK_Home:
                return Key::HOME;
            case kVK_End:
                return Key::END;
            case kVK_PageUp:
                return Key::PAGE_UP;
            case kVK_PageDown:
                return Key::PAGE_DOWN;
            case kVK_Delete:
                return Key::BACKSPACE;
            case kVK_Escape:
                return Key::ESCAPE;
            case kVK_Space:
                return Key::SPACE;
            case kVK_Tab:
                return Key::TAB;
            case kVK_CapsLock:
                return Key::CAPS_LOCK;
            case kVK_Shift:
                return Key::LEFT_SHIFT;
            case kVK_RightShift:
                return Key::RIGHT_SHIFT;
            case kVK_Control:
                return Key::LEFT_CONTROL;
            case kVK_RightControl:
                return Key::RIGHT_CONTROL;
            case kVK_Option:
                return Key::LEFT_ALT;
            case kVK_RightOption:
                return Key::RIGHT_ALT;
            case kVK_Command:
                return Key::LEFT_OS_LOGO;
            case kVK_RightCommand:
                return Key::RIGHT_OS_LOGO;
            default:
                return Key::UNKNOWN;
        }
    }

    MetaKeyFlags::Type translateMeta(uint32_t meta)
    {
        uint32_t translated = MetaKeyFlags::NONE;

        if (meta & NSEventModifierFlagOption) {
            translated |= MetaKeyFlags::ALT;
        }

        if (meta & NSEventModifierFlagControl) {
            translated |= MetaKeyFlags::CTRL;
        }

        if (meta & NSEventModifierFlagShift) {
            translated |= MetaKeyFlags::SHIFT;
        }

        if (meta & NSEventModifierFlagCommand) {
            translated |= MetaKeyFlags::OS_LOGO;
        }

        return MetaKeyFlags::Type(translated);
    }

} // namespace pm

#endif // PM_OS_MACOS

