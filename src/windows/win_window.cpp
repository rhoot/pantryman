//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include "../host.hpp"
#include "win_window.hpp"

#include <cstring>

namespace pm
{

    namespace
    {

        constexpr wchar_t WND_CLASS_NAME[] = L"pm::Window";
        constexpr uint32_t ERR_GROUP = uint32_t('wndw');

    } // namespace

    Error ERR_CLASS_REGISTRATION_FAILED{ERR_GROUP + 1, "window class registration failed"};
    Error ERR_WINDOW_CREATION_FAILED{ERR_GROUP + 2, "window creation failed"};

    uint32_t WinWindow::s_classRefs{0};

    WinWindow::WinWindow()
        : m_events{nullptr}
        , m_hwnd{nullptr}
        , m_handle{UINT8_MAX}
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
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
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

        m_handle = args.handle;
        m_events = events;
        SetWindowLongPtrW(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
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
                    WindowState state = WindowState::NORMAL;

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
            }
        }

        return DefWindowProcW(hwnd, msg, wparam, lparam);
    }

} // namespace pm
