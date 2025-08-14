///     ,.  .-,--. .
///    / |   '|__/ |
///   /~~|-. .|    |
/// ,'   `-' `'    `--'

#pragma once

#include <atomic>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace apl::sync
{

/// An object protected by a double-buffer mechanism supporting SPSC concurrency
/// 
/// @details
/// Both reading and writing are wait-free.
template<class T>
class DoubleBuffer final {
  struct alignas(128) Aligned {
    T value;
  };

  Aligned m_buffer[2]{};
  std::atomic<bool> m_should_swap{false};
  std::atomic<uint8_t> m_index{0};

public:
  DoubleBuffer() requires std::is_default_constructible_v<T> = default;
  DoubleBuffer(T initial_value);

  DoubleBuffer(DoubleBuffer&&) noexcept = delete;
  DoubleBuffer(DoubleBuffer const&) = delete;
  auto operator=(DoubleBuffer&&) noexcept = delete;
  auto operator=(const DoubleBuffer&) = delete;

  auto get() -> T;
  auto get_ref() -> const T&;
  void set(T value) requires std::is_copy_assignable_v<T> || std::is_move_assignable_v<T>;

private:
  size_t read_index() noexcept;
  size_t write_index() noexcept;
  void update_indices() noexcept;
};

/// ,-_/ ,-,-,-.  .-,--. .
/// '  | `,| | |   '|__/ |
/// .- |   | ; | . .|    |
/// `--'   '   `-' `'    `--'

template<class T>
DoubleBuffer<T>::DoubleBuffer(T initial_value):
  m_buffer{initial_value, initial_value} {}

template<class T>
auto DoubleBuffer<T>::get() -> T {
  return m_buffer[read_index()].value;
}

template<class T>
auto DoubleBuffer<T>::get_ref() -> const T& {
  return m_buffer[read_index()].value;
}

template<class T>
void DoubleBuffer<T>::set(T value)
  requires std::is_copy_assignable_v<T> || std::is_move_assignable_v<T> {
  m_buffer[write_index()].value = std::move(value);
  m_should_swap.store(true, std::memory_order::release);
}

template<class T>
auto DoubleBuffer<T>::read_index() noexcept -> size_t {
  update_indices();
  return m_index.load(std::memory_order::acquire);
}

template<class T>
auto DoubleBuffer<T>::write_index() noexcept -> size_t {
  return m_index.load(std::memory_order::acquire) ^ 1;
}

template<class T>
void DoubleBuffer<T>::update_indices() noexcept {
  if (auto expect = true; m_should_swap.compare_exchange_strong(expect, false, std::memory_order::acq_rel)) {
    m_index.fetch_xor(1, std::memory_order::acq_rel);
  }
}

}
