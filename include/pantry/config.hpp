//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

// OS

#define PANTRY_OS_WINDOWS 0

#if defined(_WIN32)
#	undef  PANTRY_OS_WINDOWS
#	define PANTRY_OS_WINDOWS 1
#endif

// Calling convention

#if PANTRY_OS_WINDOWS
#	define PANTRY_CALL __stdcall
#else
#	define PANTRY_CALL
#endif
