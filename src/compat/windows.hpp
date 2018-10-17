//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/platform.hpp>

#if defined(PM_OS_WINDOWS)

#   if !defined(WIN32_LEAN_AND_MEAN)
#       define WIN32_LEAN_AND_MEAN
#   endif

#   define NOMINMAX

#   include <Windows.h>

// wtf windows?
#   undef DELETE

#endif
