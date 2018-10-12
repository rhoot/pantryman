//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/config.hpp>
#include <pantryman/main.hpp>

#include <thread>

#include "host.hpp"

int main(int argc, char** argv)
{
    int32_t exitCode = 0;
    pm::Host host;

    const auto appProc = [&exitCode, &host, argc, argv] ()
    {
        exitCode = pmMain(argc, argv);
        host.stop();
    };

    std::thread appThread{std::move(appProc)};
    host.run();
    appThread.join();
    return exitCode;
}
