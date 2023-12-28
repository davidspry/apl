//! apl
//! David Spry
//! Created on 15/10/2023

#pragma once

#include <concepts>
#include <utility>

namespace apl {

static constexpr auto even = [](const std::integral auto value) {
  return (value & 1u) == 0;
};

static constexpr auto odd = [](const std::integral auto value) {
  return !even(value);
};

static constexpr auto eq = []<class T>(T&& sentinel) {
  return [&]<std::equality_comparable_with<T> U>(const U value) {
    return value == std::forward<T>(sentinel);
  };
};

static constexpr auto ne = []<class T>(T&& sentinel) {
  return [&]<std::equality_comparable_with<T> U>(const U value) {
    return value != std::forward<T>(sentinel);
  };
};

static constexpr auto lt = []<class T>(T&& sentinel) {
  return [&]<std::totally_ordered_with<T> U>(const U value) {
    return value < std::forward<T>(sentinel);
  };
};

static constexpr auto leq = []<class T>(T&& sentinel) {
  return [&]<std::totally_ordered_with<T> U>(const U value) {
    return value <= std::forward<T>(sentinel);
  };
};

static constexpr auto gt = []<class T>(T&& sentinel) {
  return [&]<std::totally_ordered_with<T> U>(const U value) {
    return value > std::forward<T>(sentinel);
  };
};

static constexpr auto geq = []<class T>(T&& sentinel) {
  return [&]<std::totally_ordered_with<T> U>(const U value) {
    return value >= std::forward<T>(sentinel);
  };
};

}
