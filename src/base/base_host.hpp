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

        bool processEvent(HostEvent* event);
        void run();
        void stop();

    protected:

        virtual void createWindowImpl(const CreateWindowArgs& args) = 0;
        virtual void destroyWindowImpl(WindowHandle handle) = 0;
        virtual void pumpEventsImpl() = 0;

        HostEventSink m_events;

    private:

        enum class Cmd : uint16_t;

        void processCmds();
        void processCreateWindow(const CreateWindowArgs& args);
        void processDestroyWindow(WindowHandle handle);
        void processStop();

        HostCommands  m_commands;
        bool          m_stop;

        HandlePool<WindowHandle, PM_CONFIG_MAX_WINDOWS> m_windowHandles;

    };

} // namespace pm
