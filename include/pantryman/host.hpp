//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include "window.hpp"

namespace pm
{

    struct WindowEvent
    {
        WindowHandle handle;
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
    };

    bool nextEvent(HostEvent* event);

} // namespace pm
