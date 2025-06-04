///     ,.  .-,--. .
///    / |   '|__/ |
///   /~~|-. .|    |
/// ,'   `-' `'    `--'

#pragma once

#include <apl/meta/spin_wait.h>

#include <atomic>

namespace apl::sync
{

/// A lock-free spin-lock implemented with progressive back-off
class Spinlock final {
  std::atomic_flag m_locked = ATOMIC_FLAG_INIT;

public:
  constexpr Spinlock() = default;
  Spinlock(Spinlock const&) = delete;
  Spinlock(Spinlock&&) noexcept = delete;

  void lock() noexcept;
  void unlock() noexcept;
  bool try_lock() noexcept;
  bool is_locked() const noexcept;

  auto operator=(Spinlock const&) = delete;
  auto operator=(Spinlock&&) noexcept = delete;
};

/// ,-_/ ,-,-,-.  .-,--. .
/// '  | `,| | |   '|__/ |
/// .- |   | ; | . .|    |
/// `--'   '   `-' `'    `--'

inline void Spinlock::lock() noexcept {
  apl::meta::wait_with_spin_backoff<8, 128, 512>([this]() {
    return try_lock();
  });
}

inline void Spinlock::unlock() noexcept {
  m_locked.clear(std::memory_order::release);
}

inline bool Spinlock::try_lock() noexcept {
  return !m_locked.test_and_set(std::memory_order::acquire);
}

inline bool Spinlock::is_locked() const noexcept {
  return m_locked.test(std::memory_order::acquire);
}

}
