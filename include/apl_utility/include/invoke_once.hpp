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

template<class F, class... A>
requires std::is_invocable_r_v<void, F, A...>
CONSTEXPR_AFTER_CPP23 void invoke_once(F&& invocable, A&& ... args) {
  [[maybe_unused]] static auto _ = [&invocable, ...arguments = std::forward<A>(args)] {
    std::invoke(invocable, arguments...);
    return true;
  }();
}

}
