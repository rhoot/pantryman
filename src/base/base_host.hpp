//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/host.hpp>
#include <pantryman/window.hpp>

#include "../commands.hpp"
#include "../config.hpp"
#include "../events.hpp"
#include "../handles.hpp"

#include <cstdint>

namespace pm
{

    class BaseHost
    {
    public:

        BaseHost();

        // app thread
        HostCommands& commands();
        WindowHandle createWindow(const WindowParams& params, Error* o_err);
        void destroyWindow(WindowHandle handle, Error* o_err);
        bool nextEvent(HostEvent* event);

        // host thread
        void run();

    protected:

        virtual void createWindowImpl(const CreateWindowArgs& args) = 0;
        virtual void destroyWindowImpl(WindowHandle handle) = 0;
        virtual void mainLoopImpl() = 0;
        virtual void setWindowSizeImpl(const WindowSizeArgs& args) = 0;
        virtual void setWindowStateImpl(const WindowStateArgs& args) = 0;
        virtual void setWindowStyleImpl(const WindowStyleArgs& args) = 0;

        bool frame();

        HostEventSink m_events;

    private:

        enum class Cmd : uint16_t;

        struct Callback
        {
            ExecuteFn function{nullptr};
            void*     userPointer{nullptr};
        };

        void processCmds();
        void processStop();

        HostCommands  m_commands;
        Callback      m_callbacks[PM_CONFIG_MAX_HOST_CALLBACKS];
        bool          m_stop;

        HandlePool<WindowHandle, PM_CONFIG_MAX_WINDOWS> m_windowHandles;

    };

} // namespace pm
