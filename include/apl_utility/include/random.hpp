//
// Created by David Spry on 24/2/2023.
//

#pragma once

#include <concepts>
#include <cstdint>
#include <limits>

namespace apl {

//! A fast random-number generator with constant time complexity, based on the 'xorshift64' algorithm.

class random final {
  mutable std::uint64_t m_state{};

public:
  constexpr random():
    m_state(1) {}

  constexpr explicit random(const std::uint64_t seed):
    m_state(1 + seed) {
    operator()();
  }

  constexpr auto operator()() const -> std::uint64_t {
    m_state ^= m_state << 13;
    m_state ^= m_state >> 7;
    m_state ^= m_state << 17;
    return m_state;
  }

  constexpr auto seed(const std::uint64_t seed) -> random& {
    m_state = seed;
    return *this;
  }

  //! Return true in approximately 50% of cases.
  constexpr auto chance() const -> bool {
    return 0.5f < real<float>();
  }

  //! Produce a number in the range [0, 1]
  template<std::floating_point T>
  constexpr auto real() const -> T {
    return static_cast<T>(operator()()) / static_cast<T>(std::numeric_limits<std::uint64_t>::max());
  }

  template<std::floating_point T>
  constexpr auto real(const T minimum, const T maximum) const -> T {
    return minimum + real<T>() * (maximum - minimum);
  }

  template<std::integral T>
  constexpr auto integer(const T minimum, const T maximum) const -> T {
    return static_cast<T>(real<float>(static_cast<float>(minimum), static_cast<float>(maximum)));
  }
};

}
