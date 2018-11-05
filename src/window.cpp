//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/error.hpp>
#include <pantryman/host.hpp>
#include <pantryman/window.hpp>
#include "config.hpp"
#include "host.hpp"

#include <cassert>
#include <cstring>

namespace pm
{

    namespace
    {

        struct WindowData
        {
            bool        isValid;
            bool        open;
            void*       platformHandle;
            uint16_t    width;
            uint16_t    height;
            WindowState state;
            WindowStyle style;
        };

        static WindowData s_windowDataCache[PM_CONFIG_MAX_WINDOWS];

        WindowData* getWindowDataCache(WindowHandle handle, Error* o_err)
        {
            PM_ERROR_RETURN(o_err, nullptr);

            if (handle.value >= PM_CONFIG_MAX_WINDOWS)
            {
                PM_ERROR_UPDATE(o_err, ERR_INVALID_HANDLE);
                return nullptr;
            }

            WindowData& data = s_windowDataCache[handle.value];

            if (!data.isValid)
            {
                PM_ERROR_UPDATE(o_err, ERR_INVALID_HANDLE);
                return nullptr;
            }

            return &data;
        }

    } // namespace


    void updateWindowDataCache(const HostEvent& event)
    {
        switch (event.type)
        {
            case HostEvent::WINDOW_CLOSED:
            {
                assert(event.window.handle.value < PM_CONFIG_MAX_WINDOWS);
                WindowData& data = s_windowDataCache[event.window.handle.value];
                data.open = false;
                break;
            }

            case HostEvent::WINDOW_CREATED:
            {
                const WindowCreateEvent& create = event.create;
                assert(create.handle.value < PM_CONFIG_MAX_WINDOWS);
                WindowData& data = s_windowDataCache[create.handle.value];
                data.open           = true;
                data.platformHandle = create.platformHandle;
                break;
            }

            case HostEvent::WINDOW_DESTROYED:
            {
                const WindowEvent& window = event.window;
                assert(window.handle.value < PM_CONFIG_MAX_WINDOWS);
                WindowData& data = s_windowDataCache[window.handle.value];
                std::memset(&data, 0, sizeof(data));
                break;
            }

            case HostEvent::WINDOW_RESIZED:
            {
                assert(event.resize.handle.value < PM_CONFIG_MAX_WINDOWS);
                WindowData& data = s_windowDataCache[event.resize.handle.value];
                data.width  = event.resize.width;
                data.height = event.resize.height;
                data.state  = event.resize.state;
                break;
            }

            case HostEvent::WINDOW_RESTYLED:
            {
                assert(event.style.handle.value < PM_CONFIG_MAX_WINDOWS);
                WindowData& data = s_windowDataCache[event.style.handle.value];
                data.style = event.style.style;
                break;
            }

            default:
                break;
        }
    }

    WindowHandle createWindow(const WindowParams& params, Error* o_err)
    {
        const WindowHandle handle = getHost().createWindow(params, o_err);

        if (isOk(o_err))
        {
            assert(handle.value < PM_CONFIG_MAX_WINDOWS);
            WindowData& data = s_windowDataCache[handle.value];
            data.isValid = true;
            data.width   = params.width;
            data.height  = params.height;
            data.state   = params.state;
            data.style   = params.style;
        }

        return handle;
    }

    void destroy(WindowHandle handle, Error* o_err)
    {
        getHost().destroyWindow(handle, o_err);
    }

    void setWindowSize(WindowHandle handle, uint16_t width, uint16_t height)
    {
        getHost().commands().sendSetWindowSize(handle, width, height);
    }

    void setWindowState(WindowHandle handle, WindowState state)
    {
        getHost().commands().sendSetWindowState(handle, state);
    }

    void setWindowStyle(WindowHandle handle, WindowStyle style)
    {
        getHost().commands().sendSetWindowStyle(handle, style);
    }

    void* getWindowPlatformHandle(WindowHandle handle, Error* o_err)
    {
        WindowData* data = getWindowDataCache(handle, o_err);
        return isOk(o_err) ? data->platformHandle : nullptr;
    }

    void getWindowSize(WindowHandle handle, uint16_t* o_width, uint16_t* o_height, Error* o_err)
    {
        assert(o_width != nullptr);
        assert(o_height != nullptr);

        WindowData* data = getWindowDataCache(handle, o_err);

        if (isOk(o_err))
        {
            *o_width = data->width;
            *o_height = data->height;
        }
    }

    WindowState getWindowState(WindowHandle handle, Error* o_err)
    {
        WindowData* data = getWindowDataCache(handle, o_err);
        return isOk(o_err) ? data->state : WindowState::NORMAL;
    }

    WindowStyle getWindowStyle(WindowHandle handle, Error* o_err)
    {
        WindowData* data = getWindowDataCache(handle, o_err);
        return isOk(o_err) ? data->style : WindowStyle::RESIZABLE;
    }

} // namespace pm
