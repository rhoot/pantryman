//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

// OS

#define PM_OS_WINDOWS 0

#if defined(_WIN32)
#	undef  PM_OS_WINDOWS
#	define PM_OS_WINDOWS 1
#endif

// Calling convention

#if PM_OS_WINDOWS
#	define PM_CALL __stdcall
#else
#	define PM_CALL
#endif
