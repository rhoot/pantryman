//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <cstdint>
#include <atomic>

#include <pantryman/config.hpp>

PM_DIAG_PUSH()
PM_DIAG_IGNORE_MSVC(4324) // structure was padded due to alignment specifier

namespace pm
{

    class CmdBufferSpSc
    {
    public:

        CmdBufferSpSc(uint32_t size);
        ~CmdBufferSpSc();

        // producer
        void beginWrite();
        void write(const void* data, uint16_t align, uint16_t size);
        void endWrite();

        template<class T>
        void write(const T& data)
        {
            write(&data, uint16_t(alignof(T)), uint16_t(sizeof(T)));
        }

        // consumer
        bool beginRead();
        void read(void* o_data, uint16_t align, uint16_t size);
        void endRead();

        template<class T>
        T read()
        {
            T data;
            read(&data, uint16_t(alignof(T)), uint16_t(sizeof(T)));
            return data;
        }

    private:

        // Constant shared data.
        alignas(PM_CACHE_LINE_SIZE)
        uintptr_t m_buffer;
        uint32_t  m_size;

        // Private producer data.
        alignas(PM_CACHE_LINE_SIZE)
        uint32_t  m_writePos;
        uint32_t  m_writeEnd;

        // Private consumer data.
        alignas(PM_CACHE_LINE_SIZE)
        uint32_t  m_readPos;

        // Shared producer data.
        alignas(PM_CACHE_LINE_SIZE)
        std::atomic<uint32_t> m_commit;

        // Shared consumer data.
        alignas(PM_CACHE_LINE_SIZE)
        std::atomic<uint32_t> m_consume;

    };

} // namespace pm

PM_DIAG_POP()
