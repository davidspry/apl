//
// Created by David Spry on 20/1/2023.
//

#pragma once

#include <atomic>
#include <concepts>
#include <cstdint>
#include <optional>
#include <type_traits>

namespace apl {

//! A thread-safe object-wrapper based on the 'seqlock' spin-lock algorithm.
//!
//! Writing is wait-free. Readers spin until a load can occur successfully.

template<typename T> requires std::is_trivially_copyable_v<T>
class seqlock final {
  std::atomic<std::uint64_t> m_sequence_number{};
  T m_value;

public:
  constexpr seqlock() requires std::default_initializable<T> = default;
  constexpr explicit seqlock(const T value): m_value(value) {}

  [[nodiscard]] constexpr auto load(T& value) -> T& {
    while (true) {
      const auto sequence_number1 = m_sequence_number.load(std::memory_order::acquire);

      if ((sequence_number1 & 1) != 0) {
        continue;
      }

      std::atomic_thread_fence(std::memory_order::acquire);
      std::memcpy(&value, &m_value, sizeof(T));
      const auto sequence_number2 = m_sequence_number.load(std::memory_order::relaxed);

      if (sequence_number1 != sequence_number2) {
        continue;
      }

      return value;
    }
  }

  [[nodiscard]] constexpr auto try_load(T& value) -> bool {
    const auto sequence_number1 = m_sequence_number.load(std::memory_order::acquire);

    if ((sequence_number1 & 1) != 0) {
      return false;
    }

    std::atomic_thread_fence(std::memory_order::acquire);
    std::memcpy(&value, &m_value, sizeof(T));
    const auto sequence_number2 = m_sequence_number.load(std::memory_order::relaxed);

    if (sequence_number1 != sequence_number2) {
      return false;
    }

    return true;
  }

  constexpr auto store(const T value) {
    m_sequence_number.fetch_add(1, std::memory_order::relaxed);
    std::atomic_thread_fence(std::memory_order::release);
    std::memcpy(&m_value, &value, sizeof(T));
    m_sequence_number.fetch_add(1, std::memory_order::release);
  }
};

}
