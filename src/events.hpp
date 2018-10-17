//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/error.hpp>
#include <pantryman/host.hpp>
#include <pantryman/window.hpp>
#include "cmdbuffer.hpp"

#include <cstdint>

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
        void sendWindowResizedEvent(WindowHandle handle, uint16_t w, uint16_t h, WindowState state);

    private:

        CmdBufferSpSc m_buffer;

    };

} // namespace pm
