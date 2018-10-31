//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <condition_variable>
#include <mutex>

#include "host.hpp"

namespace pm
{

    static Host s_host;

    Host& getHost()
    {
        return s_host;
    }

    HostEvent::HostEvent()
    { }

    void execute(ExecuteFn function, void* userPointer)
    {
        s_host.execute(function, userPointer);
    }

    void executeSync(ExecuteFn function, void* userPointer)
    {
        struct Data
        {
            std::condition_variable cond;
            std::mutex              mutex;
            ExecuteFn               func;
            void*                   user;
        };

        const auto handler = [](void* user)
        {
            Data* data = static_cast<Data*>(user);
            data->func(data->user);
            std::lock_guard<std::mutex> lock{data->mutex};
            data->cond.notify_all();
        };

        Data data;
        data.func = function;
        data.user = userPointer;

        std::unique_lock<std::mutex> lock{data.mutex};
        s_host.execute(handler, &data);
        data.cond.wait(lock);
    }

    bool nextEvent(HostEvent* event)
    {
        return s_host.nextEvent(event);
    }

    void stop()
    {
        s_host.stop();
    }

}
