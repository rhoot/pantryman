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
        NSWindowStyleMask style = convertStyle(args.style);

        NSWindow* window = [[NSWindow alloc]
            initWithContentRect:rect
            styleMask:style
            backing:NSBackingStoreBuffered
            defer:NO
        ];

        if (!window)
        {
            PM_ERROR_UPDATE(o_err, ERR_FAILED);
            return;
        }

        NSString* title = PM_AUTORELEASE([[NSString alloc] initWithCString:args.title encoding:NSUTF8StringEncoding]);

        [window setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
        [window setTitle:title];
        [window makeKeyAndOrderFront:nil];
        [NSApp activateIgnoringOtherApps:YES];

        m_events = events;
        m_handle = args.handle;
        m_window = PM_BRIDGE_RETAINED(void*, window);

        setState(args.state);
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
        NSWindow* window = PM_BRIDGE(NSWindow*, m_window);
        [window setContentSize:NSMakeSize(float(width), float(height))];
    }

    void MacWindow::setState(WindowState state)
    {
        NSWindow* window = PM_BRIDGE(NSWindow*, m_window);
        const NSWindowStyleMask style = [window styleMask];

        switch (state)
        {
            case WindowState::NORMAL:
                if ([window isMiniaturized])
                {
                    [window deminiaturize:nil];
                }

                if (style & NSWindowStyleMaskFullScreen)
                {
                    [window toggleFullScreen:nil];
                }
                break;

            case WindowState::MINIMIZED:
                [window miniaturize:nil];
                break;

            case WindowState::MAXIMIZED:
                if (!(style & NSWindowStyleMaskFullScreen))
                {
                    [window toggleFullScreen:nil];
                }

                break;
        }
    }

    void MacWindow::setStyle(WindowStyle style)
    {
        NSWindow* window = PM_BRIDGE(NSWindow*, m_window);

        NSWindowStyleMask oldStyle = [window styleMask];
        NSWindowStyleMask newStyle = convertStyle(style);

        if (oldStyle & NSWindowStyleMaskFullScreen)
        {
            newStyle |= NSWindowStyleMaskFullScreen;
        }

        [window setStyleMask:newStyle];
    }

    uint32_t MacWindow::convertStyle(WindowStyle style)
    {
        NSWindowStyleMask mask = 0;

        switch (style)
        {
            case WindowStyle::BORDERLESS:
                mask = NSWindowStyleMaskBorderless;
                break;

            case WindowStyle::FIXED_SIZE:
                mask = 0
                    | NSWindowStyleMaskTitled
                    | NSWindowStyleMaskClosable
                    | NSWindowStyleMaskMiniaturizable;
                break;

            case WindowStyle::RESIZABLE:
                mask = 0
                    | NSWindowStyleMaskTitled
                    | NSWindowStyleMaskClosable
                    | NSWindowStyleMaskMiniaturizable
                    | NSWindowStyleMaskResizable;
                break;
        }

        return uint32_t(mask);
    }

} // namespace pm

#endif // PM_OS_MACOS
