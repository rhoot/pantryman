//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/config.hpp>

#if PM_OS_WINDOWS

#include "../base/base_host.hpp"

namespace pm
{

    class WinHost : public BaseHost
    {

        void pumpEvents() override;

    };

    using Host = WinHost;

} // namespace pm

#endif // PM_OS_WINDOWS
