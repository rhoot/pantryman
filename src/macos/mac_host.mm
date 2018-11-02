// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/platform.hpp>

#if PM_OS_MACOS

#import <AppKit/AppKit.h>

#include "mac_host.hpp"
#include "mac_util.hpp"

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
        @autoreleasepool {
            [NSApp setDelegate:nil];
            PM_RELEASE(s_delegate);
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
        @autoreleasepool {
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
    }

    void MacHost::setWindowStateImpl(const WindowStateArgs& args)
    {
    }

    void MacHost::setWindowStyleImpl(const WindowStyleArgs& args)
    {
    }

} // namespace pm

#endif // PM_OS_MACOS
