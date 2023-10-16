//! apl
//! David Spry
//! Created on 16/10/2023

#pragma once

#include <cmath>
#include <concepts>
#include <type_traits>

namespace apl {

static constexpr auto abs = []<class T>(T&& value) {
  return std::abs(value);
};

static constexpr auto add = []<class T>(T&& value) {
  return [value = std::forward<T>(value)]<class U>(U&& input) {
    return std::forward<U>(input) + value;
  };
};

static constexpr auto sub = []<class T>(T&& value) {
  return [value = std::forward<T>(value)]<class U>(U&& input) {
    return std::forward<U>(input) - value;
  };
};

static constexpr auto sub_from = []<class T>(T&& value) {
  return [value = std::forward<T>(value)]<class U>(U&& input) {
    return value - std::forward<U>(input);
  };
};

static constexpr auto mul = []<class T>(T&& value) {
  return [value = std::forward<T>(value)]<class U>(U&& input) {
    return std::forward<U>(input) * value;
  };
};

static constexpr auto div = []<class T>(T&& value) {
  return [value = std::forward<T>(value)]<class U>(U&& input) {
    return std::forward<U>(input) / value;
  };
};

static constexpr auto dividing = []<class T>(T&& value) {
  return [value = std::forward<T>(value)]<class U>(U&& input) {
    return value / std::forward<U>(input);
  };
};

static constexpr auto lsh = []<class T>(T&& shift) {
  return [shift = std::forward<T>(shift)]<class U>(U&& input) {
    return std::forward<U>(input) << shift;
  };
};

static constexpr auto rsh = []<class T>(T&& shift) {
  return [shift = std::forward<T>(shift)]<class U>(U&& input) {
    return std::forward<U>(input) >> shift;
  };
};

static constexpr auto identity = []<class T>(T&& value) {
  return std::forward<T>(value);
};

}
