//
// Created by David Spry on 22/8/2022.
//

#pragma once

#include <array>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <functional>
#include <mutex>
#include <queue>
#include <semaphore>
#include <thread>
#include <utility>

namespace apl {

//! @brief A group of threads that each execute pending tasks.
//! @tparam NUMBER_OF_THREADS The number of threads to create.
//! @tparam WAIT_TIME_IN_MS The amount of time, in milliseconds, for which each thread should sleep between tasks.

template<std::size_t NUMBER_OF_THREADS, std::size_t WAIT_TIME_IN_MS = 250uz> requires (NUMBER_OF_THREADS > 0uz)
class thread_group final {
public:
    using task_type = std::function<void()>;

    thread_group() {
        m_is_running.store(true);
        for (auto i = 0uz; i < NUMBER_OF_THREADS; ++i) {
            m_threads[i] = std::thread([this, i]() {
                run(i + 1);
            });
        }
    }

    ~thread_group() {
        m_is_running.store(false);
        std::for_each(m_threads.begin(), m_threads.end(), [](std::thread& thread) {
            thread.join();
        });
    }

    bool has_active_tasks() const {
        return m_active_tasks.load() > 0;
    }

    bool has_pending_tasks() const {
        const std::scoped_lock scoped_lock{m_tasks_lock};
        return !m_pending_tasks.empty();
    }

    void submit_task(task_type&& new_task) {
        const std::scoped_lock scoped_lock{m_tasks_lock};
        m_pending_tasks.emplace(std::move(new_task));
    }

private:
    auto get_next_task() -> task_type {
        const std::scoped_lock scoped_lock{m_tasks_lock};
        auto next_task = std::move(m_pending_tasks.front());
        m_pending_tasks.pop();
        return next_task;
    }

    void run([[maybe_unused]] const std::size_t thread_id) {
        constexpr static auto wait_time = std::chrono::milliseconds(WAIT_TIME_IN_MS);

        while (m_is_running.load()) {
            if (has_pending_tasks()) {
                m_active_tasks.fetch_add(1);
                std::invoke(get_next_task());
                m_active_tasks.fetch_sub(1);
            }

            std::this_thread::sleep_for(wait_time);
        }
    }

private:
    std::atomic<bool> m_is_running{false};
    std::atomic<std::size_t> m_active_tasks{0};
    std::array<std::thread, NUMBER_OF_THREADS> m_threads;
    std::queue<task_type> m_pending_tasks;
    mutable std::mutex m_tasks_lock;
};

}