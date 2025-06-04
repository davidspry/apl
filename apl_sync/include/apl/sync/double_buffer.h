///     ,.  .-,--. .
///    / |   '|__/ |
///   /~~|-. .|    |
/// ,'   `-' `'    `--'

#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>

namespace apl::sync
{

/// A double-buffering mechanism supporting SPSC concurrency
/// 
/// @details
/// Both reading and writing are wait-free.
template<class T>
  requires std::is_trivially_copyable_v<T>
class DoubleBuffer {
  T m_buffer[2]{};
  std::atomic<uint8_t> m_index{0};

public:
  DoubleBuffer() requires std::is_default_constructible_v<T> = default;
  DoubleBuffer(T initial_value)
    : m_buffer{initial_value, initial_value}
  {}

  DoubleBuffer(DoubleBuffer&&) noexcept = delete;
  DoubleBuffer(DoubleBuffer const&) = delete;
  auto operator=(DoubleBuffer&&) noexcept = delete;
  auto operator=(const DoubleBuffer&) = delete;

  T get() const noexcept {
    const auto r = m_index.load(std::memory_order::acquire);
    return m_buffer[r];
  }

  void set(const T& value) noexcept {
    const auto w = m_index.load(std::memory_order::relaxed) ^ 1;
    std::memcpy(&m_buffer[w], &value, sizeof(T));
    m_index.store(w, std::memory_order::release);
  }
};

}
