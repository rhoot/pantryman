//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include "app.hpp"

namespace pm
{

    void AppBase::run()
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

    void AppBase::stop()
    {
        Msg msg;
        msg.type = Msg::STOP;
        m_msgQueue.push(msg);
    }

    bool AppBase::process(const Msg& msg)
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

    bool AppBase::processQueue()
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
