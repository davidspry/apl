//! apl
//! David Spry
//! Created on 17/8/2023

#pragma once

#include <type_traits>

namespace apl {

#if __cplusplus >= 202302L
  #define CONSTEXPR_AFTER_CPP23 constexpr
#else
  #define CONSTEXPR_AFTER_CPP23
#endif

template<class fn_t, class... args_t>
requires std::is_invocable_r_v<void, fn_t, args_t...>
CONSTEXPR_AFTER_CPP23 void invoke_once(fn_t&& invocable, args_t&& ... args) {
  [[maybe_unused]] static auto _ = [&invocable, ...arguments = std::forward<args_t>(args)] {
    std::invoke(invocable, arguments...);
    return true;
  }();
}

}
