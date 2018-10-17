//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/error.hpp>
#include <pantryman/keyboard.hpp>
#include <pantryman/window.hpp>

#include <cstdint>

namespace pm
{

    /// Keyboard input event.
    struct KeyboardEvent
    {
        /// Key that was part of the event.
        Key key;

        /// Whether the ALT key was held down at the time of the event.
        bool isAltDown;

        /// Whether the CTRL key was held down at the time of the event.
        bool isCtrlDown;

        /// Whether the SHIFT key was held down at the time of the event.
        bool isShiftDown;

        /// Whether an OS logo key was held down at the time of the event.
        bool isOsLogoDown;
    };

    /// Window event.
    struct WindowEvent
    {
        /// Handle of the window that triggered the event.
        WindowHandle handle;

        /// Error value for events triggered by a failure.
        Error error;
    };

    /// Window resize event.
    struct WindowResizeEvent
    {
        /// Handle of the window that triggered the event.
        WindowHandle handle;

        /// New width of the client area, in device pixels.
        uint16_t width;

        /// New height of the client area, in device pixels.
        uint16_t height;

        /// New state of the window.
        WindowState state;
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

            /// A window was resized. The `resize` field contains details about
            /// the event.
            WINDOW_RESIZED,

            /// A key was pressed. Does not repeat. The `keyboard` field
            /// contains details about the key.
            KEY_DOWN,

            /// A key was released. The `keyboard` field contains details about
            /// the key.
            KEY_UP,
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

            /// Window resize details. Provided for events of type
            /// `WINDOW_RESIZED`.
            WindowResizeEvent resize;

            /// Keyboard input detauls. Provided for events of type:
            /// - KEY_DOWN
            /// - KEY_UP
            KeyboardEvent keyboard;
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
