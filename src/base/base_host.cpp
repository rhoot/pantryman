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

    HostCommands& BaseHost::commands()
    {
        return m_commands;
    }

    WindowHandle BaseHost::createWindow(const WindowParams& params, Error* o_err)
    {
        const WindowHandle handle = m_windowHandles.obtain(o_err);
        PM_ERROR_RETURN(o_err, handle);
        m_commands.sendCreateWindow(handle, params);
        return handle;
    }

    void BaseHost::destroyWindow(WindowHandle handle, Error* o_err)
    {
        m_windowHandles.release(handle, o_err);
        PM_ERROR_RETURN(o_err);
        m_commands.sendDestroyWindow(handle);
    }

    bool BaseHost::nextEvent(HostEvent* event)
    {
        return m_events.nextEvent(event);
    }

    void BaseHost::run()
    {
        m_stop = false;
        mainLoopImpl();
    }

    bool BaseHost::frame()
    {
        processCmds();

        for (const Callback& cb : m_callbacks)
        {
            if (m_stop)
            {
                break;
            }

            if (cb.function)
            {
                cb.function(cb.userPointer);
            }
        }

        return !m_stop;
    }

    void BaseHost::processCmds()
    {
        HostCommand cmd;

        while (m_commands.nextCommand(&cmd))
        {
            switch (cmd.type)
            {
                case HostCommand::CREATE_WINDOW:    createWindowImpl(cmd.createWindow);            break;
                case HostCommand::DESTROY_WINDOW:   destroyWindowImpl(cmd.windowHandle);           break;
                case HostCommand::EXECUTE:          cmd.execute.function(cmd.execute.userPointer); break;
                case HostCommand::SET_WINDOW_SIZE:  setWindowSizeImpl(cmd.windowSize);             break;
                case HostCommand::SET_WINDOW_STATE: setWindowStateImpl(cmd.windowState);           break;
                case HostCommand::SET_WINDOW_STYLE: setWindowStyleImpl(cmd.windowStyle);           break;

                case HostCommand::SET_CALLBACK:
                    assert(cmd.callback.index < PM_CONFIG_MAX_HOST_CALLBACKS);
                    m_callbacks[cmd.callback.index].function    = cmd.callback.function;
                    m_callbacks[cmd.callback.index].userPointer = cmd.callback.userPointer;
                    break;

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
