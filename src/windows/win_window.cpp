//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/platform.hpp>

#if PM_OS_WINDOWS

#include <pantryman/keyboard.hpp>
#include "../commands.hpp"
#include "win_keyboard.hpp"
#include "win_window.hpp"

#include <cstring>

namespace pm
{

    namespace
    {

        constexpr wchar_t WND_CLASS_NAME[] = L"pm::Window";
        constexpr int32_t ERR_GROUP = int32_t('wndw');

    } // namespace

    Error ERR_CLASS_REGISTRATION_FAILED{ERR_GROUP + 1, "window class registration failed"};
    Error ERR_WINDOW_CREATION_FAILED{ERR_GROUP + 2, "window creation failed"};

    uint32_t WinWindow::s_classRefs{0};

    WinWindow::WinWindow()
        : m_events{nullptr}
        , m_hwnd{nullptr}
        , m_handle{UINT8_MAX}
        , m_metaKeys{MetaKeyFlags::NONE}
    { }

    void WinWindow::create(HostEventSink* events, const CreateWindowArgs& args, Error* o_err)
    {
        registerClass(o_err);
        PM_ERROR_RETURN(o_err);

        wchar_t title[MAX_WINDOW_TITLE_LEN + 1] = {0};
        MultiByteToWideChar(CP_UTF8, 0, args.title, -1, title, MAX_WINDOW_TITLE_LEN + 1);

        m_hwnd = CreateWindowExW(
            0,
            WND_CLASS_NAME,
            title,
            convertStyle(0, args.style),
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            args.width,
            args.height,
            nullptr,
            nullptr,
            GetModuleHandleW(nullptr),
            nullptr
        );

        if (!m_hwnd)
        {
            PM_ERROR_UPDATE(o_err, ERR_WINDOW_CREATION_FAILED);
            unregisterClass();
        }

        DWORD state = SW_SHOW;

        if (args.state == WindowState::MINIMIZED)
        {
            state = SW_SHOWMINIMIZED;
        }
        else if (args.state == WindowState::MAXIMIZED)
        {
            state = SW_SHOWMAXIMIZED;
        }

        m_handle = args.handle;
        m_events = events;
        SetWindowLongPtrW(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
        ShowWindow(m_hwnd, state);
    }

    bool WinWindow::isCreated() const
    {
        return m_hwnd != nullptr;
    }

    void WinWindow::destroy()
    {
        if (m_hwnd)
        {
            DestroyWindow(m_hwnd);
            unregisterClass();
            m_hwnd = nullptr;
        }

        m_events = nullptr;
    }

    void WinWindow::setSize(uint16_t width, uint16_t height)
    {
        if (!m_hwnd)
        {
            return;
        }

        POINT topLeft;
        topLeft.x = 0;
        topLeft.y = 0;

        if (!ClientToScreen(m_hwnd, &topLeft))
        {
            return;
        }

        const LONG_PTR style   = GetWindowLongPtrW(m_hwnd, GWL_STYLE);
        const LONG_PTR exStyle = GetWindowLongPtrW(m_hwnd, GWL_EXSTYLE);

        RECT rect;
        rect.left   = topLeft.x;
        rect.top    = topLeft.y;
        rect.right  = topLeft.x + LONG(width);
        rect.bottom = topLeft.y + LONG(height);

        if (!AdjustWindowRectEx(&rect, DWORD(style), FALSE, DWORD(exStyle)))
        {
            return;
        }

        const int x = int(rect.left);
        const int y = int(rect.top);
        const int w = int(rect.right - rect.left);
        const int h = int(rect.bottom - rect.top);

        MoveWindow(m_hwnd, x, y, w, h, TRUE);
    }

    void WinWindow::setState(WindowState state)
    {
        if (!m_hwnd)
        {
            return;
        }

        switch (state)
        {
            case WindowState::NORMAL:
                ShowWindow(m_hwnd, SW_RESTORE);
                break;

            case WindowState::MINIMIZED:
                ShowWindow(m_hwnd, SW_MINIMIZE);
                break;

            case WindowState::MAXIMIZED:
                ShowWindow(m_hwnd, SW_MAXIMIZE);
                break;
        }
    }

    void WinWindow::setStyle(WindowStyle style)
    {
        if (!m_hwnd)
        {
            return;
        }

        const LONG_PTR oldStyle = GetWindowLongPtrW(m_hwnd, GWL_STYLE);
        const LONG_PTR newStyle = convertStyle(uint32_t(oldStyle), style);
        SetWindowLongPtrW(m_hwnd, GWL_STYLE, newStyle);

        // Force the Window to update according to its style.
        const UINT posFlags = SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER;
        SetWindowPos(m_hwnd, nullptr, 0, 0, 0, 0, posFlags);
    }

    uint32_t WinWindow::convertStyle(uint32_t prev, WindowStyle style)
    {
        uint32_t winStyle = prev;

        switch (style)
        {
        case WindowStyle::FIXED_SIZE:
            winStyle &= ~WS_POPUP;
            winStyle &= ~WS_OVERLAPPEDWINDOW;
            winStyle |= WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
            break;

        case WindowStyle::RESIZABLE:
            winStyle &= ~WS_POPUP;
            winStyle &= ~WS_BORDER;
            winStyle |= WS_OVERLAPPEDWINDOW;
            break;

        case WindowStyle::BORDERLESS:
            winStyle &= ~WS_BORDER;
            winStyle &= ~WS_OVERLAPPEDWINDOW;
            winStyle |= WS_POPUP;
            break;
        }

        return winStyle;
    }

    void WinWindow::registerClass(Error* o_err)
    {
        PM_ERROR_RETURN(o_err);

        if (!s_classRefs)
        {
            WNDCLASSEXW cls;
            {
                std::memset(&cls, 0, sizeof(cls));

                cls.cbSize          = sizeof(cls);
                cls.style           = CS_HREDRAW | CS_VREDRAW;
                cls.lpfnWndProc     = &WinWindow::windowProc;
                cls.hInstance       = GetModuleHandleW(nullptr);
                cls.hIcon           = LoadIcon(nullptr, IDI_APPLICATION);
                cls.hCursor         = LoadCursor(nullptr, IDC_ARROW);
                cls.hbrBackground   = (HBRUSH)GetStockObject(BLACK_BRUSH);
                cls.lpszClassName   = WND_CLASS_NAME;
            }

            if (!RegisterClassExW(&cls))
            {
                PM_ERROR_UPDATE(o_err, ERR_CLASS_REGISTRATION_FAILED);
            }
        }

        ++s_classRefs;
    }

    void WinWindow::unregisterClass()
    {
        --s_classRefs;

        if (!s_classRefs)
        {
            UnregisterClassW(WND_CLASS_NAME, GetModuleHandleW(nullptr));
        }
    }

    void WinWindow::updateMetaKeys(Key key, bool isDown)
    {
        uint16_t bit = 0;

        switch (key)
        {
            case Key::LEFT_ALT:
            case Key::RIGHT_ALT:
                bit = MetaKeyFlags::ALT;
                break;

            case Key::LEFT_CONTROL:
            case Key::RIGHT_CONTROL:
                bit = MetaKeyFlags::CTRL;
                break;

            case Key::LEFT_SHIFT:
            case Key::RIGHT_SHIFT:
                bit = MetaKeyFlags::SHIFT;
                break;

            case Key::LEFT_OS_LOGO:
            case Key::RIGHT_OS_LOGO:
                bit = MetaKeyFlags::OS_LOGO;
                break;
        }

        m_metaKeys = isDown ? (m_metaKeys | bit) : (m_metaKeys & ~bit);
    }

    /*static*/ LRESULT WinWindow::windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        WinWindow* window = (WinWindow*)GetWindowLongPtrW(hwnd, GWLP_USERDATA);

        if (window)
        {
            switch (msg)
            {
                case WM_CLOSE:
                    window->m_events->sendWindowClosedEvent(window->m_handle);
                    break;

                case WM_SIZE:
                {
                    const uint16_t width  = LOWORD(lparam);
                    const uint16_t height = HIWORD(lparam);
                    WindowState    state  = WindowState::NORMAL;

                    if (wparam == SIZE_MINIMIZED)
                    {
                        state = WindowState::MINIMIZED;
                    }
                    else if (wparam == SIZE_MAXIMIZED)
                    {
                        state = WindowState::MAXIMIZED;
                    }

                    window->m_events->sendWindowResizedEvent(window->m_handle, width, height, state);
                    break;
                }

                case WM_KEYDOWN:
                {
                    const bool previous = !!(lparam & (1 << 30));

                    if (!previous)
                    {
                        const bool extended = !!(lparam & (1 << 24));
                        const Key  key      = translateKey(uint32_t(wparam), extended);
                        window->updateMetaKeys(key, true);
                        window->m_events->sendKeyDownEvent(window->m_handle, key, MetaKeyFlags::Type(window->m_metaKeys));
                    }

                    break;
                }

                case WM_KEYUP:
                {
                    // Microsoft says this is always true for WM_KEYUP. That's a lie.
                    const bool previous = !!(lparam & (1 << 30));

                    if (previous)
                    {
                        const bool extended = !!(lparam & (1 << 24));
                        const Key  key      = translateKey(uint32_t(wparam), extended);
                        window->updateMetaKeys(key, false);
                        window->m_events->sendKeyUpEvent(window->m_handle, key, MetaKeyFlags::Type(window->m_metaKeys));
                    }

                    break;
                }
            }
        }

        return DefWindowProcW(hwnd, msg, wparam, lparam);
    }

} // namespace pm

#endif // PM_OS_WINDOWS
