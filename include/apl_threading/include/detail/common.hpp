//! apl
//! David Spry
//! Created on 19/8/2023

#pragma once

#include <concepts>
#include <type_traits>

namespace apl::detail {

template<typename T>
concept assignable = std::is_copy_assignable<T>::value || std::is_move_assignable<T>::value;

}
