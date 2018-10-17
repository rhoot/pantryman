//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/error.hpp>
#include <pantryman/window.hpp>

#include <cstdint>

namespace pm
{

    /// Window event.
    struct WindowEvent
    {
        /// Handle of the window that triggered the event.
        WindowHandle handle;

        /// Error value for events triggered by a failure.
        Error error;
    };

    /// Host event.
    struct HostEvent
    {
        /// Event type.
        enum Type : uint16_t
        {
            /// The host's event loop was stopped. No more commands will be
            /// processed, and no more events will be triggered.
            STOPPED,

            /// A window was closed. It still exists, and still needs to be
            /// destroyed. The `window` field contains details about the
            /// window.
            WINDOW_CLOSED,

            /// Window creation completed. This is also triggered in case of
            /// failures. The `window` field contains details about the window,
            /// and the error in case of a failure.
            WINDOW_CREATED,

            /// Window destruction completed. The `window` field contains
            /// details about the window. Note that at the time this event is
            /// received, the handle might have been re-used and referring to
            /// a different window.
            WINDOW_DESTROYED,
        };

        /// Type of event that was triggered.
        Type type;

        union
        {
            /// Window details. Provided for events of type:
            /// - WINDOW_CLOSED
            /// - WINDOW_CREATED
            /// - WINDOW_DESTROYED
            WindowEvent window;
        };

        HostEvent();
    };

    /// Attempt to get the next host event. If an event was received, return
    /// true. If no event was available, return false.
    bool nextEvent(HostEvent* event);

    /// Stop the host. Once this has been called, no more commands will be
    /// processed.
    void stop();

} // namespace pm
