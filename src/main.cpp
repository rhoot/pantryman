//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/host.hpp>
#include <pantryman/platform.hpp>
#include <pantryman/main.hpp>

#include <thread>

#include "host.hpp"

int main(int argc, char** argv)
{
    int32_t exitCode = 0;

    const auto appProc = [&exitCode, argc, argv] ()
    {
        exitCode = pmMain(argc, argv);
        pm::stop();
    };

    std::thread appThread{std::move(appProc)};
    pm::getHost().run();
    appThread.join();
    return exitCode;
}
