//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/host.hpp>
#include <pantryman/window.hpp>

#include "../cmdbuffer.hpp"
#include "../config.hpp"
#include "../handles.hpp"
#include "base_window.hpp"

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
        virtual void destroyWindowImpl(uint8_t index) = 0;
        virtual void pumpEventsImpl() = 0;

        void sendWindowClosedEvent(uint8_t index);
        void sendWindowCreatedEvent(uint8_t index);
        void sendWindowDestroyedEvent(uint8_t index);

    private:

        enum class Cmd : uint16_t;

        void processCmds();
        void processCreateWindow();
        void processDestroyWindow();
        void processStop();

        CmdBufferSpSc m_commands;
        CmdBufferSpSc m_events;
        bool          m_stop;

        HandlePool<WindowHandle, PM_CONFIG_MAX_WINDOWS> m_windowHandles;

    };

} // namespace pm
