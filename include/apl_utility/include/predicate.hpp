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

// =============================================================================
static constexpr auto eq = []<class T>(T&& sentinel) {
  return [sentinel = std::forward<T>(sentinel)]<std::totally_ordered_with<T> U>(const U value) {
    return value == sentinel;
  };
};

static constexpr auto lt = []<class T>(T&& sentinel) {
  return [sentinel = std::forward<T>(sentinel)]<std::totally_ordered_with<T> U>(const U value) {
    return value < sentinel;
  };
};

static constexpr auto leq = []<class T>(T&& sentinel) {
  return [sentinel = std::forward<T>(sentinel)]<std::totally_ordered_with<T> U>(const U value) {
    return value <= sentinel;
  };
};

static constexpr auto gt = []<class T>(T&& sentinel) {
  return [sentinel = std::forward<T>(sentinel)]<std::totally_ordered_with<T> U>(const U value) {
    return value > sentinel;
  };
};

static constexpr auto geq = []<class T>(T&& sentinel) {
  return [sentinel = std::forward<T>(sentinel)]<std::totally_ordered_with<T> U>(const U value) {
    return value >= sentinel;
  };
};

}
