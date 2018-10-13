//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include "base_host.hpp"

namespace pm
{

    enum class BaseHost::Cmd : uint16_t
    {
        STOP,
    };

    BaseHost::BaseHost()
        : m_commands{1024}
        , m_stop{false}
    {
    }

    void BaseHost::run()
    {
        m_stop = false;

        for (;;)
        {
            processCmds();
            pumpEvents();

            if (m_stop)
            {
                break;
            }
        }
    }

    void BaseHost::stop()
    {
        m_commands.beginWrite();
        m_commands.write(Cmd::STOP);
        m_commands.endWrite();
    }

    void BaseHost::processCmds()
    {
        while (m_commands.beginRead())
        {
            Cmd cmd = m_commands.read<Cmd>();

            switch (cmd)
            {
                case Cmd::STOP:
                    processStop();
                    break;

                default:
                    break;
            }

            m_commands.endRead();
        }
    }

    void BaseHost::processStop()
    {
        m_stop = true;
    }

}
