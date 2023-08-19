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
  class loop_body_fn_t
> requires (FIRST_INDEX < FINAL_INDEX)
constexpr void constexpr_for(loop_body_fn_t&& loop_body_fn) {
  [&loop_body_fn]<std::size_t... INDEX>(const std::index_sequence<INDEX...>) {
    (loop_body_fn.template operator()<static_cast<std::int64_t>(INDEX) + FIRST_INDEX>(), ...);
  }(std::make_index_sequence<FINAL_INDEX - FIRST_INDEX>());
}

template<
  std::size_t RANGE_FINAL_INDEX,
  class loop_body_fn_t
> requires (RANGE_FINAL_INDEX > 0)
constexpr void constexpr_for(loop_body_fn_t&& loop_body_fn) {
  [&loop_body_fn]<std::size_t... INDEX>(const std::index_sequence<INDEX...>) {
    (loop_body_fn.template operator()<static_cast<std::size_t>(INDEX)>(), ...);
  }(std::make_index_sequence<RANGE_FINAL_INDEX>());
}

template<auto... T, class loop_body_fn_t>
constexpr void constexpr_for_each(const loop_body_fn_t loop_body_fn) {
  (loop_body_fn.template operator()<T>(), ...);
}

template<class... T, class loop_body_fn_t>
constexpr void constexpr_for_each(const loop_body_fn_t loop_body_fn) {
  (loop_body_fn.template operator()<T>(), ...);
}

}
