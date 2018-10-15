//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/error.hpp>

#include <cstdint>

namespace pm
{

    struct WindowHandle
    {
        uint8_t value;
    };

    struct WindowParams
    {
        uint16_t    width{1600};
        uint16_t    height{900};
        const char* title{""};
    };

    WindowHandle createWindow(const WindowParams& params, Error* o_err);
    void destroy(WindowHandle handle, Error* o_err);

} // namespace pm
