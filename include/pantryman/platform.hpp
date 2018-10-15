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

// Compiler

#define PM_COMPILER_MSVC 0

#if defined(_MSC_VER)
#   undef  PM_COMPILER_MSVC
#   define PM_COMPILER_MSVC 1
#endif

// Calling convention

#if PM_OS_WINDOWS
#	define PM_CALL __stdcall
#else
#	define PM_CALL
#endif

// Diagnostics

#if PM_COMPILER_MSVC
#   define PM_DIAG_PUSH()           __pragma(warning(push))
#   define PM_DIAG_IGNORE_MSVC(x)   __pragma(warning(disable: x))
#   define PM_DIAG_POP()            __pragma(warning(pop))
#endif

// CPU

#define PM_CACHE_LINE_SIZE 64
