//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/platform.hpp>

#if PM_OS_WINDOWS

#include "../base/base_host.hpp"
#include "../compat/windows.hpp"
#include "../config.hpp"

namespace pm
{

    class WinHost : public BaseHost
    {
    public:

        WinHost();
        ~WinHost();

    private:

        void createWindowImpl(const CreateWindowArgs& args) override;
        void destroyWindowImpl(uint8_t index) override;
        void pumpEventsImpl() override;

        static LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

        HINSTANCE m_module;
        HWND      m_windows[PM_CONFIG_MAX_WINDOWS];

    };

    using Host = WinHost;

} // namespace pm

#endif // PM_OS_WINDOWS
