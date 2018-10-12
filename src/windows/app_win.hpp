//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/config.hpp>

#if PM_OS_WINDOWS

#include "../app.hpp"

namespace pm
{

    class WinApp : public AppBase
    {

        void pumpEvents() override;

    };

    using App = WinApp;

} // namespace pm

#endif // PM_OS_WINDOWS
