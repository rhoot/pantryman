//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/platform.hpp>

#if PM_OS_MACOS

#import <AppKit/AppKit.h>

#include "mac_host.hpp"

namespace pm
{

    MacHost::MacHost()
    {
        [NSApplication sharedApplication];
        [NSApp finishLaunching];
    }

    MacHost::~MacHost()
    {}

    void MacHost::createWindowImpl(const CreateWindowArgs& args)
    {
    }

    void MacHost::destroyWindowImpl(WindowHandle handle)
    {
    }

    void MacHost::mainLoopImpl()
    {
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
