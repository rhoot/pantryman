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

    bool BaseHost::processEvent(HostEvent* event)
    {
        return m_events.nextEvent(event);
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
        m_commands.sendStopCmd();
    }

    void BaseHost::processCmds()
    {
        HostCommand cmd;

        while (m_commands.nextCommand(&cmd))
        {
            switch (cmd.type)
            {
                case HostCommand::CREATE_WINDOW:  processCreateWindow(cmd.createWindow);  break;
                case HostCommand::DESTROY_WINDOW: processDestroyWindow(cmd.windowHandle); break;
                case HostCommand::STOP:           processStop();                          break;

                default:
                    assert(!"Invalid command type.");
                    _Exit(1);
            }
        }
    }

    void BaseHost::processCreateWindow(const CreateWindowArgs& args)
    {
        createWindowImpl(args);
    }

    void BaseHost::processDestroyWindow(WindowHandle handle)
    {
        destroyWindowImpl(handle);
    }

    void BaseHost::processStop()
    {
        m_stop = true;
        m_events.sendStoppedEvent();
    }

}
