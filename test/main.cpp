//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/host.hpp>
#include <pantryman/main.hpp>
#include <pantryman/window.hpp>

#include <cstdio>

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
                case pm::HostEvent::STOPPED:
                    std::printf("STOPPED\n");
                    running = false;
                    break;

                case pm::HostEvent::WINDOW_CLOSED:
                    std::printf("WINDOW_CLOSED { handle=%#hx }\n", event.window.handle.value);
                    pm::destroy(window, pm::ErrorAssert{});
                    pm::stop();
                    break;

                case pm::HostEvent::WINDOW_CREATED:
                    std::printf("WINDOW_CREATED { handle=%#hx, error=%s }\n", event.window.handle.value, event.window.error.description);
                    break;

                case pm::HostEvent::WINDOW_DESTROYED:
                    std::printf("WINDOW_DESTROYED { handle=%#hx }\n", event.window.handle.value);
                    break;

                case pm::HostEvent::WINDOW_RESIZED:
                    std::printf("WINDOW_RESIZED { handle=%#hx, width=%hu, height=%hu, state=%hu }\n", event.resize.handle.value, event.resize.width, event.resize.height, event.resize.state);
                    break;
            }
        }
    }

	return 0;
}
