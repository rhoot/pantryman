//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/config.hpp>

#if PM_OS_WINDOWS

#if !defined(WIN32_LEAN_AND_MEAN)
#   define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

#include "win_host.hpp"

namespace pm
{

    void WinHost::pumpEvents()
    {
        MSG msg;

        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

}

#endif // PM_OS_WINDOWS
