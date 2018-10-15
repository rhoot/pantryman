//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <cstdint>

namespace pm
{

    struct Error
    {
        int32_t     code{0};
        const char* description{"OK"};
    };

    struct ErrorAssert
    {
        Error err;
        ~ErrorAssert();
        operator Error*();
    };

    extern Error ERR_OK;

    bool isOk(Error* err);

#define PM_ERROR_UPDATE(err, value)     \
    do                                  \
    {                                   \
        if (isOk(err))                  \
        {                               \
            *err = value;               \
        }                               \
    } while(false)

#define PM_ERROR_RETURN(err, ...)       \
    do                                  \
    {                                   \
        if (!isOk(err))                 \
        {                               \
            return __VA_ARGS__;         \
        }                               \
    } while(false)

} // namespace pm
