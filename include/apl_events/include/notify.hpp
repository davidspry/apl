//
// Created by David Spry on 23/3/2023.
//

#pragma once

#include <type_traits>

#include "detail/notify_detail.hpp"
#include "detail/shared_detail.hpp"

namespace apl {

template<auto enum_value, detail::enum_notifiable<decltype(enum_value)>... notifiable_types>
requires std::is_enum_v<decltype(enum_value)>
static constexpr auto notify(notifiable_types&& ... targets) {
  (targets.template notify<enum_value>(), ...);
}

template<detail::state_type T, detail::state_notifiable<T>... notifiable_types>
static constexpr auto notify(notifiable_types&& ... targets) {
  (targets.template notify<T>(), ...);
}

}
