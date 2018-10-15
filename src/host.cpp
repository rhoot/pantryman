//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include "host.hpp"

namespace pm
{

    static Host s_host;

    Host& getHost()
    {
        return s_host;
    }

    bool nextEvent(HostEvent* event)
    {
        return s_host.processEvent(event);
    }

}
