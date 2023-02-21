//
// Created by David Spry on 5/10/2022.
//

#pragma once

#include <apl/types/detail/common.hpp>
#include <concepts>
#include <iterator>

namespace apl::detail {

template<typename value_type>
concept default_constructable = std::is_default_constructible<value_type>::value;

template<typename value_type>
concept iterable = requires(const value_type& type) {
    { std::begin(type) };
    { std::end(type) };
    { std::size(type) };
};

}
