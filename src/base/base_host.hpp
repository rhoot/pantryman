//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <cstdint>

#include "../cmdbuffer.hpp"

namespace pm
{

    class BaseHost abstract
    {
    public:

        BaseHost();

        void run();
        void stop();

    protected:

        virtual void pumpEvents() = 0;

    private:

        enum class Cmd : uint16_t;

        void processCmds();
        void processStop();

        CmdBufferSpSc m_commands;
        bool          m_stop;

    };

} // namespace pm
