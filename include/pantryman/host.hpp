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
        /// Window the key event occurred in.
        WindowHandle window;

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

    /// Generic window event.
    struct WindowEvent
    {
        /// Handle of the window that triggered the event.
        WindowHandle handle;
    };

    struct WindowCreateEvent
    {
        /// Handle of the window that triggered the event.
        WindowHandle handle;

        /// Platform handle of the window.
        void* platformHandle;

        /// An error object, set to any error that might have occurred during
        /// the creation.
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

    /// Window restyle event.
    struct WindowStyleEvent
    {
        /// Handle of the window that triggered the event.
        WindowHandle handle;

        /// New style of the window.
        WindowStyle  style;
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
            /// failures. The `create` field contains details about the window,
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

            /// A window's style was changed. The `style` field contains details
            /// about the event.
            WINDOW_RESTYLED,

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
            /// - WINDOW_DESTROYED
            WindowEvent window;

            /// Window create details. Provided for events of type
            /// `WINDOW_CREATED`.
            WindowCreateEvent create;

            /// Window resize details. Provided for events of type
            /// `WINDOW_RESIZED`.
            WindowResizeEvent resize;

            /// Window style details. Provided for events of type
            /// `WINDOW_RESTYLED`.
            WindowStyleEvent style;

            /// Keyboard input detauls. Provided for events of type:
            /// - KEY_DOWN
            /// - KEY_UP
            KeyboardEvent keyboard;
        };

        HostEvent();
    };

    /// Function pointer for `execute` and `executeSync`.
    using ExecuteFn = void(*)(void*);

    /// Error returned when an invalid callback index was specified.
    extern Error ERR_INVALID_INDEX;

    /// Execute a function on the host thread. This function returns
    /// immediately, while the function may get executed at a later point.
    void execute(ExecuteFn function, void* userPointer);

    /// Execute a function on the host thread, and wait for it to complete.
    ///
    /// Important: Be super careful when using this to avoid a deadlock.
    /// Prefer using `execute` whenever possible.
    void executeSync(ExecuteFn function, void* userPointer);

    /// Attempt to get the next host event. If an event was received, return
    /// true. If no event was available, return false.
    bool nextEvent(HostEvent* event);

    /// Set a function to be called every frame on the host thread. Functions
    /// set this way will be called in the order of their index, and the index
    /// must be less than the value of `PM_CONFIG_MAX_HOST_CALLBACKS`. Set the
    /// function to `nullptr` to reset it.
    ///
    /// Important: When setting function to `nullptr`, the previous function
    /// may still be called one more time due to the asynchronousity of this
    /// call.
    void setCallback(uint16_t index, ExecuteFn function, void* userPointer, Error* o_err);

    /// Stop the host. Once this has been called, no more commands will be
    /// processed.
    void stop();

} // namespace pm
