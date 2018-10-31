//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <cstdint>

namespace pm
{

    /// Error object.
    struct Error
    {
        int32_t     code{0};
        const char* description{"OK"};
    };

    /// Assertion error. Use in place of an `Error` pointer to trigger an
    /// assertion failure on errors.
    struct ErrorAssert
    {
        Error err;
        ~ErrorAssert();
        operator Error*();
    };

    /// Success error value.
    extern Error ERR_OK;

    /// Generic failure error value.
    extern Error ERR_FAILED;

    /// Return whether the given error is OK.
    bool isOk(Error* err);

    /// Set `err` to `value` if `err` is still OK.
#define PM_ERROR_UPDATE(err, value)     \
    do                                  \
    {                                   \
        if (isOk(err))                  \
        {                               \
            *err = value;               \
        }                               \
    } while(false)

    /// Return `...` if `err` is not OK.
#define PM_ERROR_RETURN(err, ...)       \
    do                                  \
    {                                   \
        if (!isOk(err))                 \
        {                               \
            return __VA_ARGS__;         \
        }                               \
    } while(false)

} // namespace pm
