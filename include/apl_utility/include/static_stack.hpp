//! apl_utility_test
//! David Spry
//! Created on 12/9/2023

#pragma once

#include <array>
#include <cstddef>
#include <optional>

namespace apl {

//! @brief A LIFO stack with a fixed capacity.

template<class T, std::size_t SIZE>
class static_stack {
  std::array<T, SIZE> m_array;
  std::size_t m_index;

public:
  // =============================================================================
  static consteval auto capacity() -> std::size_t {
    return SIZE;
  }

  auto is_full() const -> bool {
    return m_index == SIZE;
  }

  // =============================================================================
  template<class U> requires std::same_as<T, std::decay_t<U>>
  void push(U&& item) {
    if (!is_full()) {
      m_array[m_index] = std::forward<U>(item);
      m_index += 1;
    }
  }

  [[nodiscard]] auto pull() -> std::optional<T> {
    if (m_index > 0) {
      m_index -= 1;
      return m_array[m_index];
    }

    return std::nullopt;
  }
};

}