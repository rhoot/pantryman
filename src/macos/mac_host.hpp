//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/platform.hpp>

#if PM_OS_MACOS

#include "../base/base_host.hpp"
#include "../config.hpp"
#include "mac_window.hpp"

namespace pm
{

    class MacHost : public BaseHost
    {
    public:

        MacHost();
        ~MacHost();

    private:

        void createWindowImpl(const CreateWindowArgs& args) override;
        void destroyWindowImpl(WindowHandle handle) override;
        void mainLoopImpl() override;
        void setWindowSizeImpl(const WindowSizeArgs& args) override;
        void setWindowStateImpl(const WindowStateArgs& args) override;
        void setWindowStyleImpl(const WindowStyleArgs& args) override;

        MacWindow m_windows[PM_CONFIG_MAX_WINDOWS];

    };

    using Host = MacHost;

} // namespace pm

#endif // PM_OS_MACOS
