//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/platform.hpp>

#if PM_OS_MACOS

namespace pm
{

    void controllerInit();
    void controllerUpdate();
    void controllerDestroy();

} // namespace pm

#endif // PM_OS_MACOS
