//! apl
//! David Spry
//! Created on 17/8/2023

#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>

namespace apl {

template<
  std::int64_t FIRST_INDEX,
  std::int64_t FINAL_INDEX,
  class F
> requires (FIRST_INDEX < FINAL_INDEX)
constexpr void repeat(F&& fn) {
  [&fn]<std::size_t... INDEX>(const std::index_sequence<INDEX...>) {
    (std::forward<F>(fn).template operator()<static_cast<std::int64_t>(INDEX) + FIRST_INDEX>(), ...);
  }(std::make_index_sequence<FINAL_INDEX - FIRST_INDEX>());
}

template<
  std::size_t RANGE_FINAL_INDEX,
  class F
> requires (RANGE_FINAL_INDEX > 0)
constexpr void repeat(F&& fn) {
  [&fn]<std::size_t... INDEX>(const std::index_sequence<INDEX...>) {
    (std::forward<F>(fn).template operator()<INDEX>(), ...);
  }(std::make_index_sequence<RANGE_FINAL_INDEX>());
}

}
