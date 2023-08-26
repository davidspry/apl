//! apl
//! David Spry
//! Created on 26/8/2023

#pragma once

#include <cstddef>
#include <utility>

namespace apl {

template<auto... T, class loop_body_fn_t>
constexpr void unpack(const loop_body_fn_t loop_body_fn) {
  (loop_body_fn.template operator()<T>(), ...);
}

template<class... T, class loop_body_fn_t>
constexpr void unpack(const loop_body_fn_t loop_body_fn) {
  (loop_body_fn.template operator()<T>(), ...);
}

template<auto... T, class loop_body_fn_t>
constexpr void enumerate(const loop_body_fn_t loop_body_fn) {
  [fn = std::move(loop_body_fn)]<std::size_t... INDEX>(const std::index_sequence<INDEX...>) {
    (fn.template operator()<INDEX, T>(), ...);
  }(std::make_index_sequence<sizeof...(T)>());
}

template<class... T, class loop_body_fn_t>
constexpr void enumerate(const loop_body_fn_t loop_body_fn) {
  [fn = std::move(loop_body_fn)]<std::size_t... INDEX>(const std::index_sequence<INDEX...>) {
    (fn.template operator()<INDEX, T>(), ...);
  }(std::make_index_sequence<sizeof...(T)>());
}

}
