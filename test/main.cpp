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
    params.style = pm::WindowStyle::BORDERLESS;
    params.state = pm::WindowState::MAXIMIZED;

    pm::WindowHandle window = pm::createWindow(params, pm::ErrorAssert{});

    bool running = true;

    while (running)
    {
        pm::HostEvent event;

        while (pm::nextEvent(&event))
        {
            switch (event.type)
            {
                case pm::HostEvent::KEY_DOWN:
                    std::printf(
                        "KEY_DOWN { window=%#hx, key=%#hu, alt=%s, ctrl=%s, shift=%s, os=%s }\n",
                        event.keyboard.window.value,
                        uint16_t(event.keyboard.key),
                        event.keyboard.isAltDown ? "t" : "f",
                        event.keyboard.isCtrlDown ? "t" : "f",
                        event.keyboard.isShiftDown ? "t" : "f",
                        event.keyboard.isOsLogoDown ? "t" : "f"
                    );
                    break;

                case pm::HostEvent::KEY_UP:
                    std::printf(
                        "KEY_UP { window=%#hx, key=%#hu, alt=%s, ctrl=%s, shift=%s, os=%s }\n",
                        event.keyboard.window.value,
                        uint16_t(event.keyboard.key),
                        event.keyboard.isAltDown ? "t" : "f",
                        event.keyboard.isCtrlDown ? "t" : "f",
                        event.keyboard.isShiftDown ? "t" : "f",
                        event.keyboard.isOsLogoDown ? "t" : "f"
                    );

                    switch (event.keyboard.key)
                    {
                        case pm::Key::NUM_1:
                            pm::setWindowSize(window, 640, 480);
                            break;

                        case pm::Key::NUM_2:
                            pm::setWindowSize(window, 1024, 768);
                            break;

                        case pm::Key::M:
                            pm::setWindowState(window, pm::WindowState::MINIMIZED);
                            break;

                        case pm::Key::N:
                            pm::setWindowState(window, pm::WindowState::NORMAL);
                            break;

                        case pm::Key::U:
                            pm::setWindowState(window, pm::WindowState::MAXIMIZED);
                            break;

                        case pm::Key::F:
                            pm::setWindowStyle(window, pm::WindowStyle::FIXED_SIZE);
                            break;

                        case pm::Key::R:
                            pm::setWindowStyle(window, pm::WindowStyle::RESIZABLE);
                            break;

                        case pm::Key::B:
                            pm::setWindowStyle(window, pm::WindowStyle::BORDERLESS);
                            break;
                    }
                    break;

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
