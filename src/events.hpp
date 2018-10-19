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

    struct MetaKeyFlags
    {
        enum Type : uint16_t
        {
            NONE    = 0,
            ALT     = (1 << 0),
            CTRL    = (1 << 1),
            SHIFT   = (1 << 2),
            OS_LOGO = (1 << 3),
        };
    };

    class HostEventSink
    {
    public:

        HostEventSink();

        // Consumer

        bool nextEvent(HostEvent* event);

        // Producer

        void sendKeyDownEvent(WindowHandle window, Key key, MetaKeyFlags::Type meta);
        void sendKeyUpEvent(WindowHandle window, Key key, MetaKeyFlags::Type meta);
        void sendStoppedEvent();
        void sendWindowClosedEvent(WindowHandle handle);
        void sendWindowCreatedEvent(WindowHandle handle, const Error& err);
        void sendWindowDestroyedEvent(WindowHandle handle);
        void sendWindowResizedEvent(WindowHandle handle, uint16_t w, uint16_t h, WindowState state);

    private:

        CmdBufferSpSc m_buffer;

    };

} // namespace pm
