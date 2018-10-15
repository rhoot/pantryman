//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include "config.hpp"
#include "events.hpp"

#include <cassert>

namespace pm
{

    HostEventSink::HostEventSink()
        : m_buffer{PM_CONFIG_EVENT_BUFFER_SIZE}
    { }

    bool HostEventSink::nextEvent(HostEvent* event)
    {
        if (!m_buffer.beginRead())
        {
            return false;
        }

        event->type = m_buffer.read<HostEvent::Type>();

        switch (event->type)
        {
            case HostEvent::STOPPED:
                break;

            case HostEvent::WINDOW_CLOSED:
            case HostEvent::WINDOW_DESTROYED:
                event->window.handle = m_buffer.read<WindowHandle>();
                break;

            case HostEvent::WINDOW_CREATED:
                event->window.handle = m_buffer.read<WindowHandle>();
                event->window.error  = m_buffer.read<Error>();
                break;

            default:
                assert(!"invalid event type");
                break;
        }

        m_buffer.endRead();
        return true;
    }

    void HostEventSink::sendStoppedEvent()
    {
        m_buffer.beginWrite();
        m_buffer.write(HostEvent::STOPPED);
        m_buffer.endWrite();
    }

    void HostEventSink::sendWindowClosedEvent(WindowHandle handle)
    {
        m_buffer.beginWrite();
        m_buffer.write(HostEvent::WINDOW_CLOSED);
        m_buffer.write(handle);
        m_buffer.endWrite();
    }

    void HostEventSink::sendWindowCreatedEvent(WindowHandle handle, const Error& err)
    {
        m_buffer.beginWrite();
        m_buffer.write(HostEvent::WINDOW_CREATED);
        m_buffer.write(handle);
        m_buffer.write(err);
        m_buffer.endWrite();
    }

    void HostEventSink::sendWindowDestroyedEvent(WindowHandle handle)
    {
        m_buffer.beginWrite();
        m_buffer.write(HostEvent::WINDOW_DESTROYED);
        m_buffer.write(handle);
        m_buffer.endWrite();
    }

} // namespace pm
