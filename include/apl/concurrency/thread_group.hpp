//
// Created by David Spry on 22/8/2022.
//

#pragma once

#include <apl/concurrency/seqlock.hpp>
#include <apl/types/mwmr_queue.hpp>
#include <array>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

#include <fmt/format.h>

namespace apl {

//! @brief A group of threads that each execute pending tasks.
//! @tparam NUMBER_OF_THREADS The number of threads to create.
//! @tparam WAIT_TIME_IN_MS The amount of time, in milliseconds, for which each thread should sleep between tasks.
//! @note The maximum number of concurrent tasks is 64.

template<std::size_t NUMBER_OF_THREADS, std::size_t WAIT_TIME_IN_MS = 250uz> requires (NUMBER_OF_THREADS > 0uz)
class thread_group final {
public:
    using task_type = std::function<void()>;

    thread_group() {
        m_is_running.store(true);
        for (auto i = 0uz; i < NUMBER_OF_THREADS; ++i) {
            m_threads[i] = std::thread([this, i]() {
                run(i);
            });
        }
    }

    ~thread_group() {
        m_is_running.store(false);
        std::for_each(m_threads.begin(), m_threads.end(), [](std::thread& thread) {
            thread.join();
        });
    }

    auto is_busy() const -> bool {
        return m_queued_tasks.load(std::memory_order::acquire) > 0;
    }

    auto submit_task(task_type&& new_task) -> bool {
        if (!m_pending_tasks.push(std::move(new_task))) {
            return false;
        }

        m_queued_tasks.fetch_add(1, std::memory_order::release);
        return true;
    }

private:
    auto get_next_task() -> std::optional<task_type> {
        return m_pending_tasks.pop();
    }

    void run([[maybe_unused]] const std::size_t thread_id) {
        constexpr static auto wait_time = std::chrono::milliseconds(WAIT_TIME_IN_MS);

        while (m_is_running.load(std::memory_order::relaxed)) {
            if (const auto next_task = get_next_task(); next_task.has_value()) {
                std::invoke(next_task.value());
                m_queued_tasks.fetch_sub(1, std::memory_order::release);
            }

            if constexpr (WAIT_TIME_IN_MS > 0) {
                std::this_thread::sleep_for(wait_time);
            }
        }
    }

private:
    std::atomic<bool> m_is_running{false};
    std::atomic<std::size_t> m_queued_tasks{0};
    std::array<std::thread, NUMBER_OF_THREADS> m_threads;
    apl::mwmr_queue<task_type, 64> m_pending_tasks;
};

}
