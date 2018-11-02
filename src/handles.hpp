//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/error.hpp>
#include <pantryman/platform.hpp>

#include "compat/intrin.hpp"

namespace pm
{

    extern Error ERR_INVALID_HANDLE;
    extern Error ERR_OUT_OF_HANDLES;

    template<class T, uint8_t MaxCount>
    class HandlePool
    {
        static_assert(MaxCount <= 64, "MaxCount must not be higher than 64.");

    public:

        HandlePool()
            : m_avail{(MaxCount < 64) ? ((uint64_t(1) << MaxCount) - 1) : UINT64_MAX}
        { }

        T obtain(Error* o_err)
        {
            PM_ERROR_RETURN(o_err, T{});

            if (!m_avail)
            {
                PM_ERROR_UPDATE(o_err, ERR_OUT_OF_HANDLES);
                return T{};
            }

            const uint8_t index = getNextAvailable();
            m_avail &= ~(uint64_t(1) << index);
            return T{index};
        }

        void release(T handle, Error* o_err)
        {
            PM_ERROR_RETURN(o_err);

            const uint64_t bit = uint64_t(1) << handle.value;

            if (m_avail & bit)
            {
                PM_ERROR_UPDATE(o_err, ERR_INVALID_HANDLE);
                return;
            }

            m_avail |= bit;
        }

    private:

        int8_t getNextAvailable()
        {
#if PM_COMPILER_MSVC
            unsigned long index;
            _BitScanForward64(&index, m_avail);
            return int8_t(index);
#else
            return int8_t(__builtin_ctzll(m_avail));
#endif
        }

        uint64_t m_avail;

    };

} // namespace pm
