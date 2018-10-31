//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/controller.hpp>
#include <pantryman/host.hpp>
#include <pantryman/main.hpp>
#include <pantryman/window.hpp>

#include <cinttypes>
#include <cstdio>
#include <cstring>

int32_t PM_CALL pmMain(int32_t, char**)
{
    std::printf("pmMain: 1\n");
    pm::executeSync([](void*){ std::printf("pmMain: 2\n"); }, nullptr);
    std::printf("pmMain: 3\n");
    pm::execute([](void*){ std::printf("pmMain: 4\n"); }, nullptr);
    std::printf("pmMain: 5\n");

    pm::WindowParams params;
    params.title = "foo";
    params.style = pm::WindowStyle::BORDERLESS;
    params.state = pm::WindowState::MAXIMIZED;

    pm::WindowHandle window = pm::createWindow(params, pm::ErrorAssert{});

    bool running = true;

    pm::ControllerState controllers[pm::MAX_CONTROLLERS];

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

        for (uint8_t i = 0; i < pm::MAX_CONTROLLERS; ++i)
        {
            pm::Error err;
            pm::ControllerState state;
            pm::getState(i, &state, &err);

            if (!isOk(&err))
            {
                continue;
            }

            if (std::memcmp(&state, controllers + i, sizeof(state)) == 0)
            {
                continue;
            }

            std::memcpy(controllers + i, &state, sizeof(state));

            std::printf(
                "CONTROLLER_%hhu { type=%hhu, L=(%f,%f), R=(%f,%f), LT=%f, RT=%f, buttons=(",
                i,
                state.type,
                pm::stickAxis(state.leftStickX),
                pm::stickAxis(state.leftStickY),
                pm::stickAxis(state.rightStickX),
                pm::stickAxis(state.rightStickY),
                pm::triggerAxis(state.leftTrigger),
                pm::triggerAxis(state.rightTrigger)
            );

            for (int32_t j = 0; j < 16; ++j) {
                if (isDown(state.buttons, pm::ControllerButton(1 << j))) {
                    std::printf("%d,", j);
                }
            }

            std::printf(") }\n");
        }
    }

    return 0;
}
