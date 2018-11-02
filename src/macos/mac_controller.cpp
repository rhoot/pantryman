//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/platform.hpp>

#if PM_OS_MACOS

#include <pantryman/controller.hpp>

namespace pm
{

    void getState(uint8_t controller, ControllerState* o_state, Error* o_err)
    {
        PM_ERROR_UPDATE(o_err, ERR_NOT_CONNECTED);
    }

} // namespace pm

#endif // PM_OS_MACOS
