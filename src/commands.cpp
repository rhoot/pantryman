//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include "commands.hpp"
#include "config.hpp"

#include <cassert>
#include <cstring>
#include <algorithm>

namespace pm
{

    HostCommand::HostCommand()
    { }

    HostCommands::HostCommands()
        : m_buffer{PM_CONFIG_COMMAND_BUFFER_SIZE}
    { }

    bool HostCommands::nextCommand(HostCommand* cmd)
    {
        if (!m_buffer.beginRead())
        {
            return false;
        }

        cmd->type = m_buffer.read<HostCommand::Type>();

        switch (cmd->type)
        {
            case HostCommand::CREATE_WINDOW:
            {
                cmd->createWindow.handle = m_buffer.read<WindowHandle>();
                cmd->createWindow.width  = m_buffer.read<uint16_t>();
                cmd->createWindow.height = m_buffer.read<uint16_t>();
                cmd->createWindow.state  = m_buffer.read<WindowState>();
                cmd->createWindow.style   = m_buffer.read<WindowStyle>();

                const uint16_t titleLen = m_buffer.read<uint16_t>();
                m_buffer.read(cmd->createWindow.title, 1, titleLen);
                cmd->createWindow.title[titleLen] = 0;

                break;
            }

            case HostCommand::DESTROY_WINDOW:
                cmd->windowHandle = m_buffer.read<WindowHandle>();
                break;

            case HostCommand::SET_WINDOW_SIZE:
                cmd->windowSize.handle = m_buffer.read<WindowHandle>();
                cmd->windowSize.width  = m_buffer.read<uint16_t>();
                cmd->windowSize.height = m_buffer.read<uint16_t>();
                break;

            case HostCommand::SET_WINDOW_STATE:
                cmd->windowState.handle = m_buffer.read<WindowHandle>();
                cmd->windowState.state  = m_buffer.read<WindowState>();
                break;

            case HostCommand::SET_WINDOW_STYLE:
                cmd->windowStyle.handle = m_buffer.read<WindowHandle>();
                cmd->windowStyle.style  = m_buffer.read<WindowStyle>();
                break;

            case HostCommand::STOP:
                break;

            default:
                assert(!"invalid command type");
                _Exit(1);
        }

        m_buffer.endRead();
        return true;
    }

    void HostCommands::sendCreateWindowCmd(WindowHandle handle, const WindowParams& params)
    {
        uint16_t titleLen = uint16_t(std::strlen(params.title));

        if (titleLen > MAX_WINDOW_TITLE_LEN)
        {
            titleLen = MAX_WINDOW_TITLE_LEN;
        }

        m_buffer.beginWrite();
        m_buffer.write(HostCommand::CREATE_WINDOW);
        m_buffer.write(handle);
        m_buffer.write(params.width);
        m_buffer.write(params.height);
        m_buffer.write(params.state);
        m_buffer.write(params.style);
        m_buffer.write(titleLen);
        m_buffer.write(params.title, 1, titleLen);
        m_buffer.endWrite();
    }

    void HostCommands::sendDestroyWindowCmd(WindowHandle handle)
    {
        m_buffer.beginWrite();
        m_buffer.write(HostCommand::DESTROY_WINDOW);
        m_buffer.write(handle);
        m_buffer.endWrite();
    }

    void HostCommands::sendSetWindowSize(WindowHandle handle, uint16_t width, uint16_t height)
    {
        m_buffer.beginWrite();
        m_buffer.write(HostCommand::SET_WINDOW_SIZE);
        m_buffer.write(handle);
        m_buffer.write(width);
        m_buffer.write(height);
        m_buffer.endWrite();
    }

    void HostCommands::sendSetWindowState(WindowHandle handle, WindowState state)
    {
        m_buffer.beginWrite();
        m_buffer.write(HostCommand::SET_WINDOW_STATE);
        m_buffer.write(handle);
        m_buffer.write(state);
        m_buffer.endWrite();
    }

    void HostCommands::sendSetWindowStyle(WindowHandle handle, WindowStyle style)
    {
        m_buffer.beginWrite();
        m_buffer.write(HostCommand::SET_WINDOW_STYLE);
        m_buffer.write(handle);
        m_buffer.write(style);
        m_buffer.endWrite();
    }

    void HostCommands::sendStopCmd()
    {
        m_buffer.beginWrite();
        m_buffer.write(HostCommand::STOP);
        m_buffer.endWrite();
    }

} // namespace pm
