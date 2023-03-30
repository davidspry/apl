//
// Created by David Spry on 23/3/2023.
//

#pragma once

#include <apl/detail/notify_detail.hpp>
#include <apl/detail/shared_detail.hpp>
#include <type_traits>

namespace apl {

template<auto enum_value, detail::enum_notifiable<decltype(enum_value)>... notifiable_types>
requires std::is_enum<decltype(enum_value)>::value
constexpr static auto notify(notifiable_types&& ... targets) {
    (targets.template notify<enum_value>(), ...);
}

template<detail::state_type state, detail::state_notifiable<state>... notifiable_types>
constexpr static auto notify(notifiable_types&& ... targets) {
    (targets.template notify<state>(), ...);
}

}
