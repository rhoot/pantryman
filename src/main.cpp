//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/config.hpp>
#include <pantryman/main.hpp>

#include <thread>

#include "app.hpp"

int main(int argc, char** argv)
{
    int32_t exitCode = 0;
    pm::App app;

    const auto appProc = [&exitCode, &app, argc, argv] ()
    {
        exitCode = pmMain(argc, argv);
        app.stop();
    };

    std::thread appThread{std::move(appProc)};
    app.run();
    appThread.join();
    return exitCode;
}
