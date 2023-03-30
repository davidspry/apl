//
// Created by David Spry on 27/3/2023.
//

#pragma once

#include <concepts>
#include <apl/detail/common.hpp>
#include <apl/detail/shared_detail.hpp>
#include <type_traits>

namespace apl::detail {

template<typename notifiable_type, typename enum_type>
concept enum_notifiable = std::is_enum<enum_type>::value && requires(notifiable_type type) {
    { type.template notify<enum_type{}>() } -> std::same_as<void>;
};

template<typename notifiable_type, typename state_type>
concept state_notifiable = requires(notifiable_type type) {
    { detail::state_type<state_type> };
    { type.template notify<state_type>() } -> std::same_as<void>;
};

}
