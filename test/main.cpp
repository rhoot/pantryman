//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/host.hpp>
#include <pantryman/main.hpp>
#include <pantryman/window.hpp>

int32_t PM_CALL pmMain(int32_t, char**)
{
    pm::WindowHandle window = pm::createWindow(pm::WindowParams{}, pm::ErrorAssert{});

    bool running = true;

    while (running)
    {
        pm::HostEvent event;

        while (pm::nextEvent(&event))
        {
        }
    }

    pm::destroy(window, pm::ErrorAssert{});
    
	return 0;
}
