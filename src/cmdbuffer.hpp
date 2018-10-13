//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <cstdint>
#include <atomic>

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

        alignas(64)
        uintptr_t m_buffer;
        uint32_t  m_size;

        alignas(64)
        uint32_t  m_writePos;
        uint32_t  m_writeEnd;

        alignas(64)
        uint32_t  m_readPos;

        alignas(64)
        std::atomic<uint32_t> m_commit;

        alignas(64)
        std::atomic<uint32_t> m_consume;
    };

} // namespace pm
