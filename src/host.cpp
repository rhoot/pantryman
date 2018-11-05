//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <condition_variable>
#include <mutex>

#include "host.hpp"
#include "window.hpp"

namespace pm
{

    namespace
    {

        constexpr uint32_t ERR_CODE = uint32_t('host');

    } // namespace

    Error ERR_INVALID_INDEX{ERR_CODE + 1, "invalid index"};

    static Host s_host;

    Host& getHost()
    {
        return s_host;
    }

    HostEvent::HostEvent()
    { }

    void execute(ExecuteFn function, void* userPointer)
    {
        s_host.commands().sendExecute(function, userPointer);
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
        s_host.commands().sendExecute(handler, &data);
        data.cond.wait(lock);
    }

    bool nextEvent(HostEvent* event)
    {
        if (s_host.nextEvent(event))
        {
            updateWindowDataCache(*event);
            return true;
        }

        return false;
    }

    void setCallback(uint16_t index, ExecuteFn function, void* userPointer, Error* o_err)
    {
        PM_ERROR_RETURN(o_err);

        if (index >= PM_CONFIG_MAX_HOST_CALLBACKS)
        {
            PM_ERROR_UPDATE(o_err, ERR_INVALID_INDEX);
            return;
        }

        s_host.commands().sendSetCallback(index, function, userPointer);
    }

    void stop()
    {
        s_host.commands().sendStop();
    }

}
