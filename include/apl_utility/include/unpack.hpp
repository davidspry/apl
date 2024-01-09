//! apl
//! David Spry
//! Created on 26/8/2023

#pragma once

#include <cstddef>
#include <utility>

namespace apl {

template<auto... T, class F>
constexpr void unpack(F&& fn) {
  (std::forward<F>(fn).template operator()<T>(), ...);
}

template<class... T, class F>
constexpr void unpack(F&& fn) {
  (std::forward<F>(fn).template operator()<T>(), ...);
}

template<auto... T, class F>
constexpr void enumerate(F&& fn) {
  [&fn]<std::size_t... INDEX>(const std::index_sequence<INDEX...>) {
    (std::forward<F>(fn).template operator()<INDEX, T>(), ...);
  }(std::make_index_sequence<sizeof...(T)>());
}

template<class... T, class F>
constexpr void enumerate(F&& fn) {
  [&fn]<std::size_t... INDEX>(const std::index_sequence<INDEX...>) {
    (std::forward<F>(fn).template operator()<INDEX, T>(), ...);
  }(std::make_index_sequence<sizeof...(T)>());
}

}
