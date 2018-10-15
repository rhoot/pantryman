//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/window.hpp>
#include "host.hpp"

namespace pm
{

    WindowHandle createWindow(const WindowParams& params, Error* o_err)
    {
        return getHost().createWindow(params, o_err);
    }

    void destroy(WindowHandle handle, Error* o_err)
    {
        return getHost().destroyWindow(handle, o_err);
    }

} // namespace pm
