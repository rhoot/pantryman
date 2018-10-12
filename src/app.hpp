//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include "queue.hpp"

namespace pm
{

    class AppBase abstract
    {
    public:

        void run();
        void stop();

    protected:

        virtual void pumpEvents() = 0;

    private:

        struct Msg
        {
            enum Type
            {
                STOP,
            } type;
        };

        bool process(const Msg& msg);
        bool processQueue();

        Queue<Msg> m_msgQueue;

    };

} // namespace pm

#include "windows/app_win.hpp"
