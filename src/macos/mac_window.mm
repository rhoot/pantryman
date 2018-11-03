//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/platform.hpp>

#if PM_OS_MACOS

#import <AppKit/AppKit.h>

#include "mac_keyboard.hpp"
#include "mac_util.hpp"
#include "mac_window.hpp"

@interface pmMacWindow : NSWindow

    @property(nonatomic) pm::MacWindow* owner;

@end

@implementation pmMacWindow

    -(id)initWithMacWindow:(pm::MacWindow*)window clientSize:(NSSize)size styleMask:(NSWindowStyleMask)style
    {
        const NSRect rect = NSMakeRect(0, 0, size.width, size.height);

        self = [super
            initWithContentRect:rect
            styleMask:style
            backing:NSBackingStoreBuffered
            defer:NO
        ];

        if (self)
        {
            [self setOwner:window];
        }

        return self;
    }

    -(BOOL)canBecomeKeyWindow
    {
        return YES;
    }

    -(BOOL)canBecomeMainWindow
    {
        return YES;
    }

    -(void)sendEvent:(NSEvent*)event
    {
        void* eventPtr = PM_BRIDGE(void*, event);

        switch ([event type])
        {
            case NSEventTypeKeyDown:
                [self owner]->processKeyDownEvent(eventPtr);
                return;

            case NSEventTypeKeyUp:
                [self owner]->processKeyUpEvent(eventPtr);
                return;

            default:
                break;
        }

        [super sendEvent:event];
    }

    -(BOOL)windowShouldClose:(NSWindow *)sender
    {
        [self setIsVisible:NO];
        [self owner]->processClosedEvent();
        return NO;
    }

@end

namespace pm
{

    MacWindow::MacWindow()
        : m_window{nullptr}
    {}

    void MacWindow::create(HostEventSink* events, const CreateWindowArgs& args, Error* o_err)
    {
        const NSSize size = NSMakeSize(float(args.width), float(args.height));
        NSWindowStyleMask style = convertStyle(args.style);
        pmMacWindow* window = [[pmMacWindow alloc] initWithMacWindow:this clientSize:size styleMask:style];

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
            pmMacWindow* window = PM_BRIDGE_TRANSFER(pmMacWindow*, m_window);

            if ([window isReleasedWhenClosed])
            {
                [window close];
            }
            else
            {
                [window close];
                PM_RELEASE(window);
            }

            m_window = nullptr;
        }
    }

    void MacWindow::setSize(uint16_t width, uint16_t height)
    {
        pmMacWindow* window = PM_BRIDGE(pmMacWindow*, m_window);
        [window setContentSize:NSMakeSize(float(width), float(height))];
    }

    void MacWindow::setState(WindowState state)
    {
        pmMacWindow* window = PM_BRIDGE(pmMacWindow*, m_window);
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
        pmMacWindow* window = PM_BRIDGE(pmMacWindow*, m_window);

        NSWindowStyleMask oldStyle = [window styleMask];
        NSWindowStyleMask newStyle = convertStyle(style);

        if (oldStyle & NSWindowStyleMaskFullScreen)
        {
            newStyle |= NSWindowStyleMaskFullScreen;
        }

        [window setStyleMask:newStyle];
    }

    void MacWindow::processClosedEvent()
    {
        m_events->sendWindowClosedEvent(m_handle);
    }

    void MacWindow::processKeyDownEvent(void* eventPtr)
    {
        NSEvent*                 event = PM_BRIDGE(NSEvent*, eventPtr);
        const Key                key   = translateKey([event keyCode]);
        const MetaKeyFlags::Type meta  = translateMeta(uint32_t([event modifierFlags]));

        m_events->sendKeyDownEvent(m_handle, key, meta);
    }

    void MacWindow::processKeyUpEvent(void* eventPtr)
    {
        NSEvent*                 event = PM_BRIDGE(NSEvent*, eventPtr);
        const Key                key   = translateKey([event keyCode]);
        const MetaKeyFlags::Type meta  = translateMeta(uint32_t([event modifierFlags]));
        
        m_events->sendKeyUpEvent(m_handle, key, meta);
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
