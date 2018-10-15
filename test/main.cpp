//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/host.hpp>
#include <pantryman/main.hpp>
#include <pantryman/window.hpp>

int32_t PM_CALL pmMain(int32_t, char**)
{
    pm::WindowParams params;
    params.title = "foo";

    pm::WindowHandle window = pm::createWindow(params, pm::ErrorAssert{});

    bool running = true;

    while (running)
    {
        pm::HostEvent event;

        while (pm::nextEvent(&event))
        {
            switch (event.type)
            {
                case pm::HostEvent::WINDOW_CLOSED:
                    pm::destroy(window, pm::ErrorAssert{});
                    pm::stop();
                    break;

                case pm::HostEvent::STOPPED:
                    running = false;
                    break;
            }
        }
    }

	return 0;
}
