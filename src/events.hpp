//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/error.hpp>
#include <pantryman/host.hpp>
#include "cmdbuffer.hpp"

namespace pm
{

    class HostEventSink
    {
    public:

        HostEventSink();

        // Consumer

        bool nextEvent(HostEvent* event);

        // Producer

        void sendStoppedEvent();
        void sendWindowClosedEvent(WindowHandle handle);
        void sendWindowCreatedEvent(WindowHandle handle, const Error& err);
        void sendWindowDestroyedEvent(WindowHandle handle);

    private:

        CmdBufferSpSc m_buffer;

    };

} // namespace pm
