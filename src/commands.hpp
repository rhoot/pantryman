//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/host.hpp>
#include <pantryman/window.hpp>

#include "cmdbuffer.hpp"

namespace pm
{

    constexpr uint16_t MAX_WINDOW_TITLE_LEN = 255;

    struct CreateWindowArgs
    {
        WindowHandle handle;
        uint16_t     width;
        uint16_t     height;
        WindowState  state;
        WindowStyle   style;
        char         title[MAX_WINDOW_TITLE_LEN + 1];
    };

    struct ExecuteArgs
    {
        ExecuteFn function;
        void*     userPointer;
    };

    struct WindowSizeArgs
    {
        WindowHandle handle;
        uint16_t     width;
        uint16_t     height;
    };

    struct WindowStateArgs
    {
        WindowHandle handle;
        WindowState  state;
    };

    struct WindowStyleArgs
    {
        WindowHandle handle;
        WindowStyle  style;
    };

    struct HostCommand
    {
        enum Type : uint16_t
        {
            CREATE_WINDOW,
            DESTROY_WINDOW,
            EXECUTE,
            SET_WINDOW_SIZE,
            SET_WINDOW_STATE,
            SET_WINDOW_STYLE,
            STOP,
        };

        Type type;

        union
        {
            CreateWindowArgs createWindow;
            ExecuteArgs      execute;
            WindowHandle     windowHandle;
            WindowSizeArgs   windowSize;
            WindowStateArgs  windowState;
            WindowStyleArgs  windowStyle;
        };

        HostCommand();
    };

    class HostCommands
    {
    public:

        HostCommands();

        // Consumer

        bool nextCommand(HostCommand* cmd);

        // Producer

        void sendCreateWindowCmd(WindowHandle handle, const WindowParams& params);
        void sendDestroyWindowCmd(WindowHandle handle);
        void sendExecute(ExecuteFn function, void* userPointer);
        void sendSetWindowSize(WindowHandle handle, uint16_t width, uint16_t height);
        void sendSetWindowState(WindowHandle handle, WindowState state);
        void sendSetWindowStyle(WindowHandle handle, WindowStyle style);
        void sendStopCmd();

    private:

        CmdBufferSpSc m_buffer;

    };

} // namespace pm
