// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/platform.hpp>

#if PM_OS_MACOS

#import <AppKit/AppKit.h>

#include "mac_host.hpp"

@interface pmMacHostDelegate : NSObject<NSApplicationDelegate>
@end

@implementation pmMacHostDelegate

-(NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender
{
    pm::stop();
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
        @autoreleasepool {
            s_delegate = [pmMacHostDelegate new];

            [NSApplication sharedApplication];
            [NSApp setDelegate:s_delegate];
            [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

            // Menu bar.
            NSMenu* menuBar = [[NSMenu new] autorelease];
            [NSApp setMainMenu:menuBar];

            NSMenuItem* appBarItem = [[NSMenuItem new] autorelease];
            [menuBar addItem:appBarItem];

            // Application menu.
            NSMenu* appMenu = [[NSMenu new] autorelease];
            [appBarItem setSubmenu:appMenu];

            NSMenuItem* quitItem = [[[NSMenuItem alloc] initWithTitle:@"Quit" action:@selector(terminate:) keyEquivalent:@"q"] autorelease];
            [appMenu addItem:quitItem];
        }
    }

    MacHost::~MacHost()
    {
        @autoreleasepool {
            [NSApp setDelegate:nil];
            [s_delegate release];
            s_delegate = nil;
        }
    }

    void MacHost::createWindowImpl(const CreateWindowArgs& args)
    {
    }

    void MacHost::destroyWindowImpl(WindowHandle handle)
    {
    }

    void MacHost::mainLoopImpl()
    {
        [NSApp finishLaunching];

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
    }

    void MacHost::setWindowStateImpl(const WindowStateArgs& args)
    {
    }

    void MacHost::setWindowStyleImpl(const WindowStyleArgs& args)
    {
    }

} // namespace pm

#endif // PM_OS_MACOS
