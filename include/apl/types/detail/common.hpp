//
// Created by David Spry on 28/1/2023.
//

#pragma once

#include <concepts>

namespace apl::detail {

template<typename value_type, typename decay_type>
concept decays_to = std::same_as<typename std::decay_t<value_type>, decay_type>;

}
