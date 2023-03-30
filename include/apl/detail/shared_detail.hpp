//
// Created by David Spry on 22/3/2023.
//

#pragma once

#include <apl/detail/common.hpp>
#include <concepts>

namespace apl::detail {

namespace {
template<typename value_type>
concept is_true_type = requires {
    { std::same_as<value_type, std::true_type> };
};

template<typename value_type>
concept is_false_type = requires {
    { std::same_as<value_type, std::false_type> };
};

template<typename value_type>
concept is_true_type_or_false_type = is_true_type<value_type> or is_false_type<value_type>;
}

template<typename value_type>
concept state_type = requires {
    { is_true_type_or_false_type<typename value_type::atomic> };
    { !std::is_void<typename value_type::value_type>::value };
};

template<typename value_type, typename underlying_type>
concept storage_type = requires(value_type& type, underlying_type value) {
    { type.get() } -> std::same_as<underlying_type>;
    { type.set(value) } -> std::same_as<void>;
};

}
