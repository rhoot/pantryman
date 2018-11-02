//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#if __has_feature(objc_arc)
#   define PM_AUTORELEASE(x)    (x)
#   define PM_RETAIN(x)         (x)
#   define PM_RELEASE(x)        ((void)(x))
#else
#   define PM_AUTORELEASE(x)    [(x) autorelease]
#   define PM_RETAIN(x)         [(x) retain]
#   define PM_RELEASE(x)        [(x) release]
#endif
