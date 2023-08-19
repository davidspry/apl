//
// Created by David Spry on 22/3/2023.
//

#pragma once

#include <concepts>

namespace apl::detail {

namespace {
template<typename T>
concept is_true_type = requires {
  { std::same_as<T, std::true_type> };
};

template<typename T>
concept is_false_type = requires {
  { std::same_as<T, std::false_type> };
};

template<typename T>
concept is_true_type_or_false_type = is_true_type<T> or is_false_type<T>;
}

template<typename T>
concept state_type = requires {
  { is_true_type_or_false_type<typename T::atomic> };
  { !std::is_void<typename T::value_type>::value };
};

template<typename T, typename underlying_type>
concept storage_type = requires(T& type, underlying_type value) {
  { type.get() } -> std::same_as<underlying_type>;
  { type.set(value) } -> std::same_as<void>;
};

}
