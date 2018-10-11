//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/config.hpp>
#include <pantryman/main.hpp>

#include <thread>

int main(int argc, char** argv)
{
    int32_t exitCode = 0;

    const auto appProc = [&exitCode, argc, argv] ()
    {
        exitCode = pmMain(argc, argv);
    };

    std::thread appThread{std::move(appProc)};
    appThread.join();
    return exitCode;
}
