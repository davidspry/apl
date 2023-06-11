//! David Spry
//! apl_tests

#pragma once

#include <concepts>
#include <cstdint>
#include <utility>

namespace apl {

template<std::int64_t FIRST_INDEX, std::int64_t FINAL_INDEX, class loop_body_fn> requires (FIRST_INDEX < FINAL_INDEX)
constexpr void constexpr_for(loop_body_fn&& loop_body) {
    [&loop_body]<std::size_t... INDICES>(const std::index_sequence<INDICES...>) {
        (loop_body.template operator()<static_cast<std::int64_t>(INDICES) + FIRST_INDEX>(), ...);
    }(std::make_index_sequence<FINAL_INDEX - FIRST_INDEX>());
}

template<auto... T, class loop_body_fn>
constexpr void constexpr_for_each(const loop_body_fn loop_body) {
    (loop_body.template operator()<T>(), ...);
}

template<class... T, class loop_body_fn>
constexpr void constexpr_for_each(const loop_body_fn loop_body) {
    (loop_body.template operator()<T>(), ...);
}

template<class fn, class... args> requires std::is_invocable_r_v<void, fn, args...>
constexpr void invoke_once(fn&& invocable, args&& ... arguments) {
    [[maybe_unused]] static auto _ = [
        &invocable,
        ...arguments = std::forward<args>(arguments)
    ] {
        std::invoke(invocable, arguments...);
        return true;
    }();
}

}
