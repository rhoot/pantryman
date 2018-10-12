//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include "base_host.hpp"

namespace pm
{

    void BaseHost::run()
    {
        for (;;)
        {
            if (!processQueue())
            {
                break;
            }

            pumpEvents();
        }
    }

    void BaseHost::stop()
    {
        Msg msg;
        msg.type = Msg::STOP;
        m_msgQueue.push(msg);
    }

    bool BaseHost::process(const Msg& msg)
    {
        switch (msg.type)
        {
        case Msg::STOP:
            return false;

        default:
            break;
        }

        return true;
    }

    bool BaseHost::processQueue()
    {
        Msg msg;

        while (m_msgQueue.pop(&msg))
        {
            if (!process(msg))
            {
                return false;
            }
        }

        return true;
    }

}
