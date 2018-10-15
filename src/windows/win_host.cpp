//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/platform.hpp>

#if PM_OS_WINDOWS

#include "../compat/windows.hpp"
#include "win_host.hpp"

#include <cassert>
#include <cstring>

namespace pm
{

    constexpr wchar_t WINDOW_CLASS_NAME[] = L"pm::Window";

    WinHost::WinHost()
        : m_module{GetModuleHandleW(nullptr)}
    {
        std::memset(m_windows, 0, sizeof(m_windows));

        WNDCLASSEXW cls;
        {
            std::memset(&cls, 0, sizeof(cls));

            cls.cbSize          = sizeof(cls);
            cls.style           = CS_HREDRAW | CS_VREDRAW;
            cls.lpfnWndProc     = &WinHost::windowProc;
            cls.hInstance       = m_module;
            cls.hIcon           = LoadIcon(nullptr, IDI_APPLICATION);
            cls.hCursor         = LoadCursor(nullptr, IDC_ARROW);
            cls.hbrBackground   = (HBRUSH)GetStockObject(BLACK_BRUSH);
            cls.lpszClassName   = WINDOW_CLASS_NAME;
        }

        RegisterClassExW(&cls);
    }

    WinHost::~WinHost()
    {
        UnregisterClassW(WINDOW_CLASS_NAME, m_module);
    }

    void WinHost::createWindowImpl(const CreateWindowArgs& args)
    {
        assert(!m_windows[args.index]);

        wchar_t title[MAX_WINDOW_TITLE_LEN + 1];
        MultiByteToWideChar(CP_UTF8, 0, args.title, -1, title, MAX_WINDOW_TITLE_LEN + 1);

        HWND hwnd = CreateWindowExW(
            0,
            WINDOW_CLASS_NAME,
            title,
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            args.width,
            args.height,
            nullptr,
            nullptr,
            m_module,
            nullptr
        );

        m_windows[args.index] = hwnd;
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)this);
    }

    void WinHost::destroyWindowImpl(uint8_t index)
    {
        assert(m_windows[index]);
        DestroyWindow(m_windows[index]);
        m_windows[index] = nullptr;
    }

    void WinHost::pumpEventsImpl()
    {
        MSG msg;

        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    LRESULT WinHost::windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        WinHost* host = (WinHost*)GetWindowLongPtrW(hwnd, GWLP_USERDATA);

        if (host)
        {
            uint8_t index = UINT8_MAX;

            for (uint8_t i = 0; i < PM_CONFIG_MAX_WINDOWS; ++i)
            {
                if (host->m_windows[i] == hwnd)
                {
                    index = i;
                    break;
                }
            }

            switch (msg)
            {
                case WM_CLOSE:
                    if (index != UINT8_MAX)
                    {
                        host->sendWindowClosedEvent(index);
                    }
                    break;
            }
        }

        return DefWindowProcW(hwnd, msg, wparam, lparam);
    }

}

#endif // PM_OS_WINDOWS
