//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/config.hpp>

#if PM_OS_WINDOWS

#include "../host.hpp"

namespace pm
{

    class WinHost : public HostBase
    {

        void pumpEvents() override;

    };

    using Host = WinHost;

} // namespace pm

#endif // PM_OS_WINDOWS
