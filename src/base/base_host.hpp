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

    class BaseHost abstract
    {
    public:

        BaseHost();

        WindowHandle createWindow(const WindowParams& params, Error* o_err);
        void destroyWindow(WindowHandle handle, Error* o_err);
        void execute(ExecuteFn function, void* userPointer);
        void setWindowSize(WindowHandle handle, uint16_t width, uint16_t height);
        void setWindowState(WindowHandle handle, WindowState state);
        void setWindowStyle(WindowHandle handle, WindowStyle style);

        bool nextEvent(HostEvent* event);
        void run();
        void stop();

    protected:

        virtual void createWindowImpl(const CreateWindowArgs& args) = 0;
        virtual void destroyWindowImpl(WindowHandle handle) = 0;
        virtual void pumpEventsImpl() = 0;
        virtual void setWindowSizeImpl(const WindowSizeArgs& args) = 0;
        virtual void setWindowStateImpl(const WindowStateArgs& args) = 0;
        virtual void setWindowStyleImpl(const WindowStyleArgs& args) = 0;

        HostEventSink m_events;

    private:

        enum class Cmd : uint16_t;

        void processCmds();
        void processStop();

        HostCommands  m_commands;
        bool          m_stop;

        HandlePool<WindowHandle, PM_CONFIG_MAX_WINDOWS> m_windowHandles;

    };

} // namespace pm
