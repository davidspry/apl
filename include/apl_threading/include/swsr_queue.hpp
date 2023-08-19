//! David Spry
//! apl

#pragma once

#include <array>
#include <atomic>
#include <bit>
#include <cstddef>
#include <optional>

#include "detail/common.hpp"

namespace apl {

//! A queue supporting concurrent use by a single writer and a single reader.

template<detail::assignable T, std::size_t CAPACITY> requires (std::has_single_bit(CAPACITY))
class swsr_queue {
  std::array<T, CAPACITY> m_array{};
  std::atomic<std::size_t> m_size{0};
  std::atomic<std::size_t> m_enqueue_index{0};
  std::atomic<std::size_t> m_dequeue_index{0};
  static constexpr std::size_t m_array_bounds_mask = CAPACITY - 1;

public:
  constexpr swsr_queue() requires std::is_default_constructible<T>::value = default;

  constexpr auto is_full() const -> bool {
    return m_size.load(std::memory_order::acquire) == CAPACITY;
  }

  constexpr auto is_empty() const -> bool {
    return m_size.load(std::memory_order::acquire) == 0;
  }

  //! Push a new item into the queue.
  //! @returns A boolean to indicate the success or failure of the operation.
  constexpr auto push(T value) -> bool {
    if (!is_full()) {
      m_size.fetch_add(1, std::memory_order::release);
      const auto index = m_enqueue_index.load(std::memory_order::acquire);
      m_array[index] = std::move(value);
      m_enqueue_index.store((index + 1) & m_array_bounds_mask, std::memory_order::release);
      return true;
    } else {
      return false;
    }
  }

  //! Pop the next item from the queue.
  //! @returns An optional containing either the next item or nullopt if the queue was empty.
  constexpr auto pop() -> std::optional<T> {
    if (!is_empty()) {
      m_size.fetch_sub(1, std::memory_order::release);
      const auto index = m_dequeue_index.load(std::memory_order::acquire);
      const auto value = m_array[index];
      m_dequeue_index.store((index + 1) & m_array_bounds_mask, std::memory_order::release);
      return value;
    } else {
      return std::nullopt;
    }
  }
};

}
