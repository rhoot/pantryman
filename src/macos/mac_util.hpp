//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#if __has_feature(objc_arc)
#   define PM_AUTORELEASE(x)        (x)
#   define PM_RETAIN(x)             (x)
#   define PM_RELEASE(x)            ((void)(x))
#   define PM_BRIDGE(t, x)          ((__bridge t)(x))
#   define PM_BRIDGE_RETAINED(t, x) ((__bridge_retained t)(x))
#   define PM_BRIDGE_TRANSFER(t, x) ((__bridge_transfer t)(x))
#else
#   define PM_AUTORELEASE(x)        [(x) autorelease]
#   define PM_RETAIN(x)             [(x) retain]
#   define PM_RELEASE(x)            [(x) release]
#   define PM_BRIDGE(t, x)          ((t)(x))
#   define PM_BRIDGE_RETAINED(t, x) ((t)(x))
#   define PM_BRIDGE_TRANSFER(t, x) ((t)(x))
#endif
