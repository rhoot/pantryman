//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include "base_host.hpp"

#include <cassert>
#include <algorithm>

namespace pm
{

    enum class BaseHost::Cmd : uint16_t
    {
        CREATE_WINDOW,
        DESTROY_WINDOW,
        STOP,
    };

    BaseHost::BaseHost()
        : m_commands{2048}
        , m_events{2048}
        , m_stop{false}
    {
    }

    WindowHandle BaseHost::createWindow(const WindowParams& params, Error* o_err)
    {
        const WindowHandle handle = m_windowHandles.obtain(o_err);
        PM_ERROR_RETURN(o_err, handle);

        const uint16_t titleLen = std::min(uint16_t(255), uint16_t(std::strlen(params.title)));

        m_commands.beginWrite();
        m_commands.write(Cmd::CREATE_WINDOW);
        m_commands.write(handle.value);
        m_commands.write(params.width);
        m_commands.write(params.height);
        m_commands.write(titleLen);
        m_commands.write(params.title, alignof(char), titleLen);
        m_commands.endWrite();

        return handle;
    }

    void BaseHost::destroyWindow(WindowHandle handle, Error* o_err)
    {
        m_windowHandles.release(handle, o_err);
        PM_ERROR_RETURN(o_err);

        m_commands.beginWrite();
        m_commands.write(Cmd::DESTROY_WINDOW);
        m_commands.write(handle.value);
        m_commands.endWrite();
    }

    bool BaseHost::processEvent(HostEvent* event)
    {
        if (!m_events.beginRead())
        {
            return false;
        }

        event->type = m_events.read<HostEvent::Type>();

        switch (event->type)
        {
            case HostEvent::STOPPED:
                break;

            case HostEvent::WINDOW_CLOSED:
            case HostEvent::WINDOW_CREATED:
            case HostEvent::WINDOW_DESTROYED:
                event->window.handle = WindowHandle{m_events.read<uint8_t>()};
                break;

            default:
                assert(!"invalid event type");
                _Exit(1);
        }

        m_events.endRead();
        return true;
    }

    void BaseHost::run()
    {
        m_stop = false;

        for (;;)
        {
            processCmds();
            pumpEventsImpl();

            if (m_stop)
            {
                break;
            }
        }
    }

    void BaseHost::stop()
    {
        m_commands.beginWrite();
        m_commands.write(Cmd::STOP);
        m_commands.endWrite();
    }

    void BaseHost::sendWindowClosedEvent(uint8_t index)
    {
        m_events.beginWrite();
        m_events.write(HostEvent::WINDOW_CLOSED);
        m_events.write(index);
        m_events.endWrite();
    }

    void BaseHost::sendWindowCreatedEvent(uint8_t index)
    {
        m_events.beginWrite();
        m_events.write(HostEvent::WINDOW_CREATED);
        m_events.write(index);
        m_events.endWrite();
    }

    void BaseHost::sendWindowDestroyedEvent(uint8_t index)
    {
        m_events.beginWrite();
        m_events.write(HostEvent::WINDOW_DESTROYED);
        m_events.write(index);
        m_events.endWrite();
    }

    void BaseHost::processCmds()
    {
        while (m_commands.beginRead())
        {
            Cmd cmd = m_commands.read<Cmd>();

            switch (cmd)
            {
                case Cmd::CREATE_WINDOW:  processCreateWindow();  break;
                case Cmd::DESTROY_WINDOW: processDestroyWindow(); break;
                case Cmd::STOP:           processStop();          break;

                default:
                    assert(!"Invalid command type.");
                    _Exit(1);
            }
        }
    }

    void BaseHost::processCreateWindow()
    {
        CreateWindowArgs args;
        {
            args.index = m_commands.read<uint8_t>();
            args.width = m_commands.read<uint16_t>();
            args.height = m_commands.read<uint16_t>();

            const uint16_t titleLen = m_commands.read<uint16_t>();
            m_commands.read(args.title, 1, titleLen);
            args.title[titleLen] = 0;
        }

        m_commands.endRead();
        createWindowImpl(args);
    }

    void BaseHost::processDestroyWindow()
    {
        const uint8_t index = m_commands.read<uint8_t>();
        m_commands.endRead();
        destroyWindowImpl(index);
    }

    void BaseHost::processStop()
    {
        m_commands.endRead();
        m_stop = true;

        m_events.beginWrite();
        m_events.write(HostEvent::STOPPED);
        m_events.endWrite();
    }

}
