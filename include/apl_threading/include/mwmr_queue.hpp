//
// Created by David Spry on 28/1/2023.
//

#pragma once

#include <array>
#include <atomic>
#include <bit>
#include <concepts>
#include <optional>

#include "detail/common.hpp"

namespace apl {

template<detail::assignable T, std::size_t CAPACITY> requires (std::has_single_bit(CAPACITY))
class mwmr_queue {
  struct occupiable {
    std::atomic_flag is_occupied{false};
    std::optional<T> value{std::nullopt};
  };

  std::array<occupiable, CAPACITY> m_array;
  std::atomic<std::size_t> m_enqueue_index{0};
  std::atomic<std::size_t> m_dequeue_index{0};
  static constexpr std::size_t m_array_bounds_mask = CAPACITY - 1;

public:
  constexpr mwmr_queue() = default;

  constexpr auto push(T value) -> bool {
    while (true) {
      auto enqueue_index = m_enqueue_index.load(std::memory_order::relaxed);
      auto& item = m_array[enqueue_index];
      const auto is_occupied = item.is_occupied.test(std::memory_order::acquire);

      if (!is_occupied) {
        if (m_enqueue_index.compare_exchange_weak(enqueue_index,
                                                  (enqueue_index + 1) & m_array_bounds_mask,
                                                  std::memory_order::relaxed)) {
          item.value = std::move(value);
          item.is_occupied.test_and_set();
          return true;
        }
      } else {
        return false;
      }
    }
  }

  constexpr auto pop() -> std::optional<T> {
    while (true) {
      auto dequeue_index = m_dequeue_index.load(std::memory_order::relaxed);
      auto& item = m_array[dequeue_index];
      const auto is_occupied = item.is_occupied.test(std::memory_order::acquire);

      if (is_occupied) {
        if (m_dequeue_index.compare_exchange_weak(dequeue_index,
                                                  (dequeue_index + 1) & m_array_bounds_mask,
                                                  std::memory_order::relaxed)) {
          const auto value = std::move(item.value);
          item.is_occupied.clear(std::memory_order::release);
          return value;
        }
      } else {
        return std::nullopt;
      }
    }
  }
};

}
