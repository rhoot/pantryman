//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/config.hpp>

#include <cstdint>
#include <atomic>
#include <thread>

PM_DIAG_PUSH()
PM_DIAG_IGNORE_MSVC(4324) // '': structure was padded due to alignment specifier

namespace pm
{

    template<class T>
    class Queue
    {
    public:

        Queue()
        {
            for (uint32_t i = 0; i < QUEUE_SIZE; ++i)
            {
                std::atomic_init(&m_slots[i].sequence, 0);
            }

            std::atomic_init(&m_push, 0);
            std::atomic_init(&m_pop, 0);
        }

        void push(const T& data)
        {
            Node* slot;
            uint32_t pos = std::atomic_load_explicit(&m_push, std::memory_order_relaxed);

            for (;;)
            {
                slot = m_slots + (pos & (QUEUE_SIZE - 1));
                const uint32_t seq = std::atomic_load_explicit(&slot->sequence, std::memory_order_acquire);
                int32_t diff = int32_t(int64_t(seq) - int64_t(pos));

                if (!diff)
                {
                    if (std::atomic_compare_exchange_weak_explicit(&m_push, &pos, pos + 1, std::memory_order_relaxed, std::memory_order_relaxed))
                    {
                        break;
                    }
                }
                else if (diff < 0)
                {
                    std::this_thread::yield();
                }
            }

            slot->data = data;
            std::atomic_store_explicit(&slot->sequence, pos + 1, std::memory_order_release);
        }

        bool pop(T* o_data)
        {
            Node* slot;
            uint32_t pos = std::atomic_load_explicit(&m_pop, std::memory_order_relaxed);

            for (;;)
            {
                slot = m_slots + (pos & (QUEUE_SIZE - 1));
                const uint32_t seq = std::atomic_load_explicit(&slot->sequence, std::memory_order_acquire);
                int32_t diff = int32_t(int64_t(seq) - int64_t(pos + 1));

                if (!diff)
                {
                    if (std::atomic_compare_exchange_weak_explicit(&m_pop, &pos, pos + 1, std::memory_order_relaxed, std::memory_order_relaxed))
                    {
                        break;
                    }
                }
                else if (diff < 0)
                {
                    return false;
                }
            }

            *o_data = slot->data;
            std::atomic_store_explicit(&slot->sequence, pos + QUEUE_SIZE + 1, std::memory_order_release);
            return true;
        }

    private:

        struct Node
        {
            std::atomic<uint32_t> sequence;
            T                     data;
        };

        enum : uint32_t { QUEUE_SIZE = 128 };

        Node                              m_slots[QUEUE_SIZE];
        alignas(64) std::atomic<uint32_t> m_push;
        alignas(64) std::atomic<uint32_t> m_pop;

    };

}

PM_DIAG_POP()
