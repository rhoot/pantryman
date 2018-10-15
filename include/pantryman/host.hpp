//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/error.hpp>
#include <pantryman/window.hpp>

#include <cstdint>

namespace pm
{

    struct WindowEvent
    {
        WindowHandle handle;
        Error        error;
    };

    struct HostEvent
    {
        enum Type : uint16_t
        {
            STOPPED,
            WINDOW_CLOSED,
            WINDOW_CREATED,
            WINDOW_DESTROYED,
        };

        Type type;

        union
        {
            WindowEvent window;
        };

        HostEvent();
    };

    bool nextEvent(HostEvent* event);
    void stop();

} // namespace pm
