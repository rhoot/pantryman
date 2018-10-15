//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

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
        char         title[MAX_WINDOW_TITLE_LEN + 1];
    };

    struct HostCommand
    {
        enum Type : uint16_t
        {
            CREATE_WINDOW,
            DESTROY_WINDOW,
            STOP,
        };

        Type type;

        union
        {
            CreateWindowArgs createWindow;
            WindowHandle     windowHandle;
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
        void sendStopCmd();

    private:

        CmdBufferSpSc m_buffer;

    };

} // namespace pm
