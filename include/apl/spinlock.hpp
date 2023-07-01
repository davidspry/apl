//! APL
//! David Spry
//! Created on 1/7/2023

#pragma once

#include <atomic>
#include <chrono>
#include <thread>

namespace apl {

//! Spinlock
//! @author Fedor Pikus
//! @see    "A Spinlock Implementation", Fedor Pikus, presented at C++ Now, 2022.
class spinlock {
    std::atomic<bool> m_flag{false};

public:
    spinlock() = default;
    explicit spinlock(const spinlock&) = delete;
    explicit spinlock(const spinlock&&) = delete;

    bool is_locked() const {
        return m_flag.load(std::memory_order::relaxed);
    }

    bool try_lock() {
        return !(m_flag.load(std::memory_order::relaxed) ||
                 m_flag.exchange(true, std::memory_order::acquire));
    }

    void lock() {
        for (auto i = 0; try_lock(); ++i) {
            if (i == 8) {
                i = 0;
                std::this_thread::sleep_for(
                    std::chrono::nanoseconds(1)
                );
            }
        }
    }

    void unlock() {
        m_flag.store(false, std::memory_order::release);
    }

    auto operator=(const spinlock&) = delete;
    auto operator=(const spinlock&&) = delete;
};

//! Scoped-spinlock
//! @brief A class that locks and unlocks a given spinlock in an RAII-fashion.
class scoped_spinlock {
    spinlock& m_spinlock_ref;

public:
    scoped_spinlock(spinlock& spinlock):
        m_spinlock_ref(spinlock) {
        m_spinlock_ref.lock();
    }

    scoped_spinlock(const scoped_spinlock&) = delete;
    scoped_spinlock(const scoped_spinlock&&) = delete;

    ~scoped_spinlock() {
        m_spinlock_ref.unlock();
    }

    auto operator=(const scoped_spinlock&) = delete;
    auto operator=(const scoped_spinlock&&) = delete;
};

}
