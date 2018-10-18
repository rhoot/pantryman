//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/platform.hpp>

#if PM_OS_WINDOWS

#include "../compat/windows.hpp"
#include "win_host.hpp"

#include <cassert>
#include <cstring>

namespace pm
{

    WinHost::WinHost()
    { }

    WinHost::~WinHost()
    { }

    void WinHost::createWindowImpl(const CreateWindowArgs& args)
    {
        assert(args.handle.value < PM_CONFIG_MAX_WINDOWS);
        WinWindow& window = m_windows[args.handle.value];
        assert(!window.isCreated());
        
        Error err;
        window.create(&m_events, args, &err);
        m_events.sendWindowCreatedEvent(args.handle, err);
    }

    void WinHost::destroyWindowImpl(WindowHandle handle)
    {
        assert(handle.value < PM_CONFIG_MAX_WINDOWS);
        WinWindow& window = m_windows[handle.value];
        assert(window.isCreated());
        window.destroy();
        m_events.sendWindowDestroyedEvent(handle);
    }

    void WinHost::pumpEventsImpl()
    {
        MSG msg;

        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

	void WinHost::setWindowStateImpl(const WindowStateArgs& args)
	{
        assert(args.handle.value < PM_CONFIG_MAX_WINDOWS);
        WinWindow& window = m_windows[args.handle.value];
        assert(window.isCreated());
        window.setState(args.state);
	}

    void WinHost::setWindowStyleImpl(const WindowStyleArgs& args)
    {
        assert(args.handle.value < PM_CONFIG_MAX_WINDOWS);
        WinWindow& window = m_windows[args.handle.value];
        assert(window.isCreated());
        window.setStyle(args.style);
    }

}

#endif // PM_OS_WINDOWS
