//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/error.hpp>

#include <cstdint>

namespace pm
{

    /// Window handle.
    struct WindowHandle
    {
        uint8_t value{UINT8_MAX};
    };

    /// Window creation parameters.
    struct WindowParams
    {
        /// Width of the window client area, in device pixels.
        uint16_t width{1600};

        /// Height of the window client area, in device pixels.
        uint16_t height{900};

        /// Title of the window.
        const char* title{""};
    };

    /// Window states.
    enum class WindowState : uint16_t
    {
        NORMAL,
        MINIMIZED,
        MAXIMIZED,
    };

    /// Asynchronously create a window. Once completed, trigger a
    /// `WINDOW_CREATED` event regardless of whether it was successful. If an
    /// error occurs before the asynchronous operation has been queued,
    /// populate `o_err` with the error that occurred.
    WindowHandle createWindow(const WindowParams& params, Error* o_err);

    /// Asynchronously destroy a window. Once completed, trigger a
    /// `WINDOW_DESTROYED` event. If an error occurs before the asynchronous
    /// operation has been queued, populate `o_err` with the error that
    /// occured.
    void destroy(WindowHandle handle, Error* o_err);

} // namespace pm
