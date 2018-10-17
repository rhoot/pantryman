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
            case HostEvent::KEY_DOWN:
            case HostEvent::KEY_UP:
            {
                const Key key                 = m_buffer.read<Key>();
                const MetaKeyFlags::Type meta = m_buffer.read<MetaKeyFlags::Type>();
                event->keyboard.key           = key;
                event->keyboard.isAltDown     = !!(meta & MetaKeyFlags::ALT);
                event->keyboard.isCtrlDown    = !!(meta & MetaKeyFlags::CTRL);
                event->keyboard.isShiftDown   = !!(meta & MetaKeyFlags::SHIFT);
                event->keyboard.isOsLogoDown  = !!(meta & MetaKeyFlags::OS_LOGO);
                break;
            }

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

            case HostEvent::WINDOW_RESIZED:
                event->resize.handle = m_buffer.read<WindowHandle>();
                event->resize.width  = m_buffer.read<uint16_t>();
                event->resize.height = m_buffer.read<uint16_t>();
                event->resize.state  = m_buffer.read<WindowState>();
                break;

            default:
                assert(!"invalid event type");
                break;
        }

        m_buffer.endRead();
        return true;
    }

    void HostEventSink::sendKeyDownEvent(Key key, MetaKeyFlags::Type meta)
    {
        m_buffer.beginWrite();
        m_buffer.write(HostEvent::KEY_DOWN);
        m_buffer.write(key);
        m_buffer.write(meta);
        m_buffer.endWrite();
    }

    void HostEventSink::sendKeyUpEvent(Key key, MetaKeyFlags::Type meta)
    {
        m_buffer.beginWrite();
        m_buffer.write(HostEvent::KEY_UP);
        m_buffer.write(key);
        m_buffer.write(meta);
        m_buffer.endWrite();
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

	void HostEventSink::sendWindowResizedEvent(WindowHandle handle, uint16_t w, uint16_t h, WindowState state)
	{
        m_buffer.beginWrite();
        m_buffer.write(HostEvent::WINDOW_RESIZED);
        m_buffer.write(handle);
        m_buffer.write(w);
        m_buffer.write(h);
        m_buffer.write(state);
        m_buffer.endWrite();
	}

} // namespace pm
