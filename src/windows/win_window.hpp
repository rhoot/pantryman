//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/platform.hpp>

#if PM_OS_WINDOWS

#include "../compat/windows.hpp"
#include "../events.hpp"

namespace pm
{

    extern Error ERR_CLASS_REGISTRATION_FAILED;
    extern Error ERR_WINDOW_CREATION_FAILED;

    class WinWindow
    {
    public:

        WinWindow();

        void create(HostEventSink* events, const CreateWindowArgs& args, Error* o_err);
        bool isCreated() const;
        void destroy();

    private:

        void registerClass(Error* o_err);
        void unregisterClass();

        static LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
        static uint32_t s_classRefs;

        HostEventSink* m_events;
        HWND           m_hwnd;
        WindowHandle   m_handle;
    };

    using Window = WinWindow;

} // namespace pm

#endif // PM_OS_WINDOWS
