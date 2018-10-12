//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include "host.hpp"

namespace pm
{

    void HostBase::run()
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

    void HostBase::stop()
    {
        Msg msg;
        msg.type = Msg::STOP;
        m_msgQueue.push(msg);
    }

    bool HostBase::process(const Msg& msg)
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

    bool HostBase::processQueue()
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
