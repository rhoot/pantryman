//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/platform.hpp>

#if PM_OS_MACOS

#import <AppKit/AppKit.h>

#include "mac_host.hpp"
#include "mac_util.hpp"
#include "mac_window.hpp"

@interface pmMacHostDelegate : NSObject<NSApplicationDelegate>
@end

@implementation pmMacHostDelegate

    -(NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender
    {
        for (NSWindow* window in [NSApp windows])
        {
            [window close];
        }

        return NSTerminateCancel;
    }

@end

namespace pm
{

    namespace
    {

        static pmMacHostDelegate* s_delegate;

    }

    MacHost::MacHost()
    {
        @autoreleasepool
        {
            s_delegate = [pmMacHostDelegate new];

            [NSApplication sharedApplication];
            [NSApp setDelegate:s_delegate];
            [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

            // Menu bar.
            NSMenu* menuBar = PM_AUTORELEASE([NSMenu new]);
            [NSApp setMainMenu:menuBar];

            NSMenuItem* appBarItem = PM_AUTORELEASE([NSMenuItem new]);
            [menuBar addItem:appBarItem];

            // Application menu.
            NSMenu* appMenu = PM_AUTORELEASE([NSMenu new]);
            [appBarItem setSubmenu:appMenu];

            NSMenuItem* quitItem = PM_AUTORELEASE([[NSMenuItem alloc] initWithTitle:@"Quit" action:@selector(terminate:) keyEquivalent:@"q"]);
            [appMenu addItem:quitItem];
        }
    }

    MacHost::~MacHost()
    {
        @autoreleasepool
        {
            [NSApp setDelegate:nil];
            PM_RELEASE(s_delegate);
            s_delegate = nil;
        }
    }

    void MacHost::createWindowImpl(const CreateWindowArgs& args)
    {
        assert(args.handle.value < PM_CONFIG_MAX_WINDOWS);
        MacWindow& window = m_windows[args.handle.value];
        assert(!window.isCreated());

        Error err;
        window.create(&m_events, args, &err);
        m_events.sendWindowCreatedEvent(args.handle, err);
    }

    void MacHost::destroyWindowImpl(WindowHandle handle)
    {
        assert(handle.value < PM_CONFIG_MAX_WINDOWS);
        MacWindow& window = m_windows[handle.value];
        assert(window.isCreated());
        window.destroy();
        m_events.sendWindowDestroyedEvent(handle);
    }

    void MacHost::mainLoopImpl()
    {
        @autoreleasepool
        {
            [NSApp finishLaunching];
        }

        for (;;)
        {
            @autoreleasepool
            {
                for (;;)
                {
                    NSEvent* event = [NSApp
                        nextEventMatchingMask:NSEventMaskAny
                        untilDate:nil
                        inMode:NSDefaultRunLoopMode
                        dequeue:YES
                    ];

                    if (event == nil)
                    {
                        break;
                    }

                    [NSApp sendEvent:event];
                    [NSApp updateWindows];
                }

                if (!frame())
                {
                    break;
                }
            }
        }
    }

    void MacHost::setWindowSizeImpl(const WindowSizeArgs& args)
    {
        assert(args.handle.value < PM_CONFIG_MAX_WINDOWS);
        MacWindow& window = m_windows[args.handle.value];
        assert(window.isCreated());
        window.setSize(args.width, args.height);
    }

    void MacHost::setWindowStateImpl(const WindowStateArgs& args)
    {
        assert(args.handle.value < PM_CONFIG_MAX_WINDOWS);
        MacWindow& window = m_windows[args.handle.value];
        assert(window.isCreated());
        window.setState(args.state);
    }

    void MacHost::setWindowStyleImpl(const WindowStyleArgs& args)
    {
        assert(args.handle.value < PM_CONFIG_MAX_WINDOWS);
        MacWindow& window = m_windows[args.handle.value];
        assert(window.isCreated());
        window.setStyle(args.style);
    }

} // namespace pm

#endif // PM_OS_MACOS
