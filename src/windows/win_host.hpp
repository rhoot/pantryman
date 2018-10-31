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
#include "win_window.hpp"

namespace pm
{

    class WinHost : public BaseHost
    {
    public:

        WinHost();
        ~WinHost();

    private:

        void createWindowImpl(const CreateWindowArgs& args) override;
        void destroyWindowImpl(WindowHandle handle) override;
        void pumpEventsImpl() override;
        void setWindowSizeImpl(const WindowSizeArgs& args) override;
        void setWindowStateImpl(const WindowStateArgs& args) override;
        void setWindowStyleImpl(const WindowStyleArgs& args) override;

        WinWindow m_windows[PM_CONFIG_MAX_WINDOWS];

    };

    using Host = WinHost;

} // namespace pm

#endif // PM_OS_WINDOWS
