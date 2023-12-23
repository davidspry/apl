//
// Created by David Spry on 24/2/2023.
//

#pragma once

#include <concepts>
#include <cstdint>
#include <limits>
#include <numbers>

namespace apl {

//! A fast random-number generator with constant time-complexity, based on the 'xoroshiro128p' algorithm.

class random final {
  std::uint64_t m_state1 = 64;
  std::uint64_t m_state2 = 96;

public:
  random() = default;

  constexpr explicit random(
    const std::uint64_t seed1,
    const std::uint64_t seed2
  ): m_state1(seed1),
     m_state2(seed2) {
    if (seed1 == 0 && seed2 == 0) {
      m_state2 += 1;
    }

    [[maybe_unused]] const auto _ = operator()();
  }

  // ============================================================================
  //! Produce the next value in the sequence.
  [[nodiscard]] constexpr auto operator()() -> std::uint64_t {
    const auto result = m_state1 + m_state2;
    const auto [state1, state2] = xoroshiro128p(m_state1, m_state2 ^ m_state1);
    m_state1 = state1;
    m_state2 = state2;
    return result;
  }

  //! Produce a value of the given unsigned type.
  template<std::unsigned_integral T>
  [[nodiscard]] constexpr auto operator()() -> T {
    return static_cast<T>(
      operator()() >> (64u - 8u * sizeof(T))
    );
  }

  template<> [[nodiscard]] constexpr auto operator()<bool>() -> bool {
    return static_cast<bool>(
      operator()() >> (63u)
    );
  }

  // ============================================================================
  //! Produce a number in the range [0, 1]
  template<std::floating_point T>
  constexpr auto uniform() -> T;

  template<> [[nodiscard]] constexpr auto uniform<float>() -> float {
    return static_cast<float>(operator()<std::uint32_t>()) /
           static_cast<float>(std::numeric_limits<std::uint32_t>::max());
  }

  template<> [[nodiscard]] constexpr auto uniform<double>() -> double {
    return static_cast<double>(operator()<std::uint64_t>()) /
           static_cast<double>(std::numeric_limits<std::uint64_t>::max());
  }

  //! Produce a number from a set of numbers with mean 0 and standard deviation 1
  template<std::floating_point T>
  [[nodiscard]] constexpr auto normal() -> T {
    const auto r1 = std::max(T{0}, uniform<T>() - std::numeric_limits<T>::epsilon());
    const auto r2 = uniform<T>();
    return std::sqrt(-T(2) * std::log(T(1) - r1)) * std::sin(T(2) * std::numbers::pi_v<T> * r2);
  }

private:
  static constexpr auto xoroshiro128p(
    const std::uint64_t state1,
    const std::uint64_t state2
  ) -> std::tuple<std::uint64_t, std::uint64_t> {
    const auto s1 = ((state1 << 55u) | (state1 >> (64u - 55u))) ^ state2 ^ (state2 << 14u);
    const auto s2 = ((state2 << 36u) | (state2 >> (64u - 36u)));
    return std::make_tuple(s1, s2);
  }
};

}
