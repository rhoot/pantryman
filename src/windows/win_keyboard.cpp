//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/platform.hpp>

#if PM_OS_WINDOWS

#include "../compat/windows.hpp"
#include "win_keyboard.hpp"

namespace pm
{

    Key translateKey(uint32_t vk, bool extended)
    {
        switch (vk)
        {
            case 'A':
                return Key::A;
            case 'B':
                return Key::B;
            case 'C':
                return Key::C;
            case 'D':
                return Key::D;
            case 'E':
                return Key::E;
            case 'F':
                return Key::F;
            case 'G':
                return Key::G;
            case 'H':
                return Key::H;
            case 'I':
                return Key::I;
            case 'J':
                return Key::J;
            case 'K':
                return Key::K;
            case 'L':
                return Key::L;
            case 'M':
                return Key::M;
            case 'N':
                return Key::N;
            case 'O':
                return Key::O;
            case 'P':
                return Key::P;
            case 'Q':
                return Key::Q;
            case 'R':
                return Key::R;
            case 'S':
                return Key::S;
            case 'T':
                return Key::T;
            case 'U':
                return Key::U;
            case 'V':
                return Key::V;
            case 'W':
                return Key::W;
            case 'X':
                return Key::X;
            case 'Y':
                return Key::Y;
            case 'Z':
                return Key::Z;
            case '0':
                return Key::NUM_0;
            case '1':
                return Key::NUM_1;
            case '2':
                return Key::NUM_2;
            case '3':
                return Key::NUM_3;
            case '4':
                return Key::NUM_4;
            case '5':
                return Key::NUM_5;
            case '6':
                return Key::NUM_6;
            case '7':
                return Key::NUM_7;
            case '8':
                return Key::NUM_8;
            case '9':
                return Key::NUM_9;
            case VK_NUMPAD0:
                return Key::NUMPAD_0;
            case VK_NUMPAD1:
                return Key::NUMPAD_1;
            case VK_NUMPAD2:
                return Key::NUMPAD_2;
            case VK_NUMPAD3:
                return Key::NUMPAD_3;
            case VK_NUMPAD4:
                return Key::NUMPAD_4;
            case VK_NUMPAD5:
                return Key::NUMPAD_5;
            case VK_NUMPAD6:
                return Key::NUMPAD_6;
            case VK_NUMPAD7:
                return Key::NUMPAD_7;
            case VK_NUMPAD8:
                return Key::NUMPAD_8;
            case VK_NUMPAD9:
                return Key::NUMPAD_9;
            case VK_DECIMAL:
                return Key::NUMPAD_DECIMAL;
            case VK_RETURN:
                return extended ? Key::NUMPAD_ENTER : Key::ENTER;
            case VK_ADD:
                return Key::NUMPAD_ADD;
            case VK_SUBTRACT:
                return Key::NUMPAD_SUBTRACT;
            case VK_MULTIPLY:
                return Key::NUMPAD_MULTIPLY;
            case VK_DIVIDE:
                return Key::NUMPAD_DIVIDE;
            case VK_NUMLOCK:
                return Key::NUM_LOCK;
            case VK_F1:
                return Key::F1;
            case VK_F2:
                return Key::F2;
            case VK_F3:
                return Key::F3;
            case VK_F4:
                return Key::F4;
            case VK_F5:
                return Key::F5;
            case VK_F6:
                return Key::F6;
            case VK_F7:
                return Key::F7;
            case VK_F8:
                return Key::F8;
            case VK_F9:
                return Key::F9;
            case VK_F10:
                return Key::F10;
            case VK_F11:
                return Key::F11;
            case VK_F12:
                return Key::F12;
            case VK_OEM_3:
                return Key::BACKTICK;
            case VK_OEM_MINUS:
                return Key::DASH;
            case VK_OEM_PLUS:
                return Key::EQUAL;
            case VK_OEM_4:
                return Key::LEFT_BRACKET;
            case VK_OEM_6:
                return Key::RIGHT_BRACKET;
            case VK_OEM_1:
                return Key::COLON;
            case VK_OEM_7:
                return Key::QUOTE;
            case VK_OEM_COMMA:
                return Key::COMMA;
            case VK_OEM_PERIOD:
                return Key::PERIOD;
            case VK_OEM_2:
                return Key::SLASH;
            case VK_UP:
                return Key::UP;
            case VK_DOWN:
                return Key::DOWN;
            case VK_LEFT:
                return Key::LEFT;
            case VK_RIGHT:
                return Key::RIGHT;
            case VK_INSERT:
                return Key::INSERT;
            case VK_DELETE:
                return Key::DELETE;
            case VK_HOME:
                return Key::HOME;
            case VK_END:
                return Key::END;
            case VK_PRIOR:
                return Key::PAGE_UP;
            case VK_NEXT:
                return Key::PAGE_DOWN;
            case VK_BACK:
                return Key::BACKSPACE;
            case VK_ESCAPE:
                return Key::ESCAPE;
            case VK_SPACE:
                return Key::SPACE;
            case VK_TAB:
                return Key::TAB;
            case VK_CAPITAL:
                return Key::CAPS_LOCK;
            case VK_SHIFT:
                return extended ? Key::RIGHT_SHIFT : Key::LEFT_SHIFT;
            case VK_LSHIFT:
                return Key::LEFT_SHIFT;
            case VK_RSHIFT:
                return Key::RIGHT_SHIFT;
            case VK_CONTROL:
                return extended ? Key::RIGHT_CONTROL : Key::LEFT_CONTROL;
            case VK_LCONTROL:
                return Key::LEFT_CONTROL;
            case VK_RCONTROL:
                return Key::RIGHT_CONTROL;
            case VK_MENU:
                return extended ? Key::RIGHT_ALT : Key::LEFT_ALT;
            case VK_LMENU:
                return Key::LEFT_ALT;
            case VK_RMENU:
                return Key::RIGHT_ALT;
            case VK_LWIN:
                return Key::LEFT_OS_LOGO;
            case VK_RWIN:
                return Key::RIGHT_OS_LOGO;
            default:
                return Key::UNKNOWN;
        }
    }

} // namespace pm

#endif // PM_OS_WINDOWS
