//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <thread>

#include "cmdbuffer.hpp"

namespace pm
{

    namespace
    {

        uintptr_t alignUp(uintptr_t value, uint16_t align)
        {
            const uintptr_t mask = (align - 1);
            return (value + mask) & ~mask;
        }

    } // namespace

    CmdBufferSpSc::CmdBufferSpSc(uint32_t size)
        : m_size{size}
        , m_writePos{0}
        , m_writeEnd{0}
        , m_readPos{0}
        , m_commit{0}
        , m_consume{0}
    {
        m_buffer = reinterpret_cast<uintptr_t>(std::malloc(size));
    }

    CmdBufferSpSc::~CmdBufferSpSc()
    {
        std::free(reinterpret_cast<void*>(m_buffer));
    }

    void CmdBufferSpSc::beginWrite()
    {
        const uint32_t consume = m_consume.load(std::memory_order_acquire);
        m_writeEnd = (consume - 1) % m_size;
    }

    void CmdBufferSpSc::write(const void* data, uint16_t align, uint16_t size)
    {
        for (;;)
        {
            const uint32_t remaining = (m_writeEnd - m_writePos) % m_size;

            uintptr_t addr  = alignUp(m_buffer + m_writePos, align);
            uint32_t  end   = uint32_t(addr - m_buffer) + size;
            uint32_t  bytes = end - m_writePos;

            if (end > m_size)
            {
                addr  = alignUp(m_buffer, align);
                end   = uint32_t(addr - m_buffer) + size;
                bytes = (m_size - m_writePos) + end;
            }

            if (bytes > remaining)
            {
                // Buffer full: Retry in a bit...
                std::this_thread::yield();
                beginWrite();
                continue;
            }

            void* dst = reinterpret_cast<void*>(addr);
            memcpy(dst, data, size);
            m_writePos = end;
            break;
        }
    }

    void CmdBufferSpSc::endWrite()
    {
        m_commit.store(m_writePos, std::memory_order_release);
    }

    bool CmdBufferSpSc::beginRead()
    {
        const uint32_t commit  = m_commit.load(std::memory_order_acquire);
        const uint32_t consume = m_consume.load(std::memory_order_relaxed);
        return commit != consume;
    }

    void CmdBufferSpSc::read(void* data, uint16_t align, uint16_t size)
    {
        uintptr_t addr = alignUp(m_buffer + m_readPos, align);
        uint32_t  end  = uint32_t(addr - m_buffer) + size;

        if (end > m_size)
        {
            addr = alignUp(m_buffer, align);
            end  = uint32_t(addr - m_buffer) + size;
        }

        const void* src = reinterpret_cast<void*>(addr);
        memcpy(data, src, size);
        m_readPos = end;
    }

    void CmdBufferSpSc::endRead()
    {
        m_consume.store(m_readPos, std::memory_order_release);
    }

} // namespace pm
