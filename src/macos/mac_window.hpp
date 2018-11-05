//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/platform.hpp>

#if PM_OS_MACOS

#include "../commands.hpp"
#include "../events.hpp"

namespace pm
{

    class MacWindow
    {
    public:

        MacWindow();

        void* create(HostEventSink* events, const CreateWindowArgs& args, Error* o_err);
        bool isCreated() const;
        void destroy();
        void setSize(uint16_t width, uint16_t height);
        void setState(WindowState state);
        void setStyle(WindowStyle style);

        void processClosedEvent();
        void processKeyDownEvent(void* event);
        void processKeyUpEvent(void* event);
        void sendResizeEvent();

    private:

        uint32_t convertStyle(WindowStyle style);

        HostEventSink*  m_events;
        void*           m_window;
        WindowHandle    m_handle;
    };

    using Window = MacWindow;

} // namespace pm

#endif // PM_OS_MACOS
