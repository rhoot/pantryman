//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include "base_host.hpp"

#include <cassert>
#include <algorithm>

namespace pm
{

    BaseHost::BaseHost()
        : m_stop{false}
    {
    }

    WindowHandle BaseHost::createWindow(const WindowParams& params, Error* o_err)
    {
        const WindowHandle handle = m_windowHandles.obtain(o_err);
        PM_ERROR_RETURN(o_err, handle);
        m_commands.sendCreateWindowCmd(handle, params);
        return handle;
    }

    void BaseHost::destroyWindow(WindowHandle handle, Error* o_err)
    {
        m_windowHandles.release(handle, o_err);
        PM_ERROR_RETURN(o_err);
        m_commands.sendDestroyWindowCmd(handle);
    }

    void BaseHost::setWindowState(WindowHandle handle, WindowState state)
    {
        m_commands.sendSetWindowState(handle, state);
    }

    void BaseHost::setWindowStyle(WindowHandle handle, WindowStyle style)
    {
        m_commands.sendSetWindowStyle(handle, style);
    }

    bool BaseHost::nextEvent(HostEvent* event)
    {
        return m_events.nextEvent(event);
    }

    void BaseHost::run()
    {
        m_stop = false;

        for (;;)
        {
            pumpEventsImpl();
            processCmds();

            if (m_stop)
            {
                break;
            }
        }
    }

    void BaseHost::stop()
    {
        m_commands.sendStopCmd();
    }

    void BaseHost::processCmds()
    {
        HostCommand cmd;

        while (m_commands.nextCommand(&cmd))
        {
            switch (cmd.type)
            {
                case HostCommand::CREATE_WINDOW:    createWindowImpl(cmd.createWindow);  break;
                case HostCommand::DESTROY_WINDOW:   destroyWindowImpl(cmd.windowHandle); break;
                case HostCommand::SET_WINDOW_STATE: setWindowStateImpl(cmd.windowState); break;
                case HostCommand::SET_WINDOW_STYLE: setWindowStyleImpl(cmd.windowStyle); break;

                case HostCommand::STOP:
                    processStop();
                    return;

                default:
                    assert(!"Invalid command type.");
                    _Exit(1);
            }
        }
    }

    void BaseHost::processStop()
    {
        m_stop = true;
        m_events.sendStoppedEvent();
    }

}
