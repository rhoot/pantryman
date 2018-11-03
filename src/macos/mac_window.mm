//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/platform.hpp>

#if PM_OS_MACOS

#import <AppKit/AppKit.h>

#include "../commands.hpp"
#include "mac_window.hpp"
#include "mac_util.hpp"

namespace pm
{

    MacWindow::MacWindow()
        : m_window{nullptr}
    {}

    void MacWindow::create(HostEventSink* events, const CreateWindowArgs& args, Error* o_err)
    {
        const NSRect rect = NSMakeRect(0, 0, float(args.width), float(args.height));

        NSWindow* window = [[NSWindow alloc]
            initWithContentRect:rect
            styleMask:NSWindowStyleMaskTitled
            backing:NSBackingStoreBuffered
            defer:NO
        ];

        if (!window)
        {
            PM_ERROR_UPDATE(o_err, ERR_FAILED);
            return;
        }

        NSString* title = PM_AUTORELEASE([[NSString alloc] initWithCString:args.title encoding:NSUTF8StringEncoding]);

        [window setTitle:title];
        [window makeKeyAndOrderFront:nil];
        [NSApp activateIgnoringOtherApps:YES];

        m_events = events;
        m_handle = args.handle;
        m_window = PM_BRIDGE_RETAINED(void*, window);
    }

    bool MacWindow::isCreated() const
    {
        return !!m_window;
    }

    void MacWindow::destroy()
    {
        if (m_window)
        {
            NSWindow* window = PM_BRIDGE_TRANSFER(NSWindow*, m_window);
            [window close];
            PM_RELEASE(window);
            m_window = nullptr;
        }
    }

    void MacWindow::setSize(uint16_t width, uint16_t height)
    {

    }

    void MacWindow::setState(WindowState state)
    {

    }

    void MacWindow::setStyle(WindowStyle style)
    {

    }

} // namespace pm

#endif // PM_OS_MACOS
