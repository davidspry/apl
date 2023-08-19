//
// Created by David Spry on 22/8/2022.
//

#pragma once

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <functional>
#include <thread>
#include <utility>

#include "mwmr_queue.hpp"
#include "seqlock.hpp"

namespace apl {

//! @brief A group of threads that each execute pending tasks.
//! @tparam NUMBER_OF_THREADS The number of threads to create.
//! @tparam WAIT_TIME_IN_MS The amount of time, in milliseconds, for which each thread should sleep between tasks.
//! @note The maximum number of concurrent tasks is 64.

template<std::size_t NUMBER_OF_THREADS, std::size_t WAIT_TIME_IN_MS = 250uz> requires (NUMBER_OF_THREADS > 0uz)
class thread_group final {
  std::array<std::thread, NUMBER_OF_THREADS> m_threads;
  std::atomic<bool> m_is_running{false};
  std::atomic<std::size_t> m_queued_tasks{0};
  apl::mwmr_queue<std::function<void()>, 64> m_pending_tasks;

public:
  using task_t = std::function<void()>;

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

  auto submit_task(task_t new_task) -> bool {
    if (!m_pending_tasks.push(std::move(new_task))) {
      return false;
    }

    m_queued_tasks.fetch_add(1, std::memory_order::release);
    return true;
  }

private:
  auto get_next_task() -> std::optional<task_t> {
    return m_pending_tasks.pop();
  }

  void run([[maybe_unused]] const std::size_t thread_id) {
    while (m_is_running.load(std::memory_order::relaxed)) {
      if (const auto next_task = get_next_task(); next_task.has_value()) {
        std::invoke(next_task.value());
        m_queued_tasks.fetch_sub(1, std::memory_order::release);
      }

      if constexpr (WAIT_TIME_IN_MS > 0) {
        std::this_thread::sleep_for(
          std::chrono::milliseconds(WAIT_TIME_IN_MS)
        );
      }
    }
  }
};

}
