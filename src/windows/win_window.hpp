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
        void setState(WindowState state);
        void setStyle(WindowStyle style);

    private:

        uint32_t convertStyle(uint32_t prev, WindowStyle style);
        void registerClass(Error* o_err);
        void unregisterClass();
        void updateMetaKeys(Key key, bool isDown);

        static LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
        static uint32_t s_classRefs;

        HostEventSink*  m_events;
        HWND            m_hwnd;
        WindowHandle    m_handle;
        uint16_t        m_metaKeys;
    };

    using Window = WinWindow;

} // namespace pm

#endif // PM_OS_WINDOWS
