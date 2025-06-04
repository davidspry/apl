///     ,.  .-,--. .
///    / |   '|__/ |
///   /~~|-. .|    |
/// ,'   `-' `'    `--'

#pragma once

#include <concepts>
#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>

namespace apl::meta
{

/// Invoke the given object N times in an unrolled loop.
/// 
/// @details
/// Equivalent to (((fn(), fn()), ...), fn())
/// 
/// @tparam N
/// The number of inline invocations
template<size_t N, std::invocable Fn>
constexpr void inline_for(Fn&& fn) {
  [&]<size_t... K>(std::index_sequence<K...>) {
    (..., (std::invoke(fn), static_cast<void>(K)));
  }(std::make_index_sequence<N>());
}

/// Invoke the given object N times in an unrolled loop,
/// instantiating its operator() template with the loop index
/// for each invocation.
/// 
/// @details
/// Equivalent to (((fn<0>(), fn<1>()), ...), fn<N-1>())
/// 
/// @tparam N
/// The number of inline invocations
template<size_t N, class Fn>
  requires requires { std::declval<Fn>().template operator()<N>(); }
constexpr void inline_for(Fn&& fn) {
  [&]<size_t... K>(std::index_sequence<K...>) {
    (..., fn.template operator()<K>());
  }(std::make_index_sequence<N>());
}

/// Invoke the given object N times in an unrolled loop
/// and compute the sum of the results.
/// 
/// @details
/// Equivalent to (fn() + fn() + ... + fn())
/// 
/// @tparam N
/// The number of inline invocations
template<size_t N, std::invocable Fn>
  requires (N != 0) && requires(std::invoke_result_t<Fn> x) { x + x; }
constexpr auto inline_sum(Fn&& fn) {
  return [&]<size_t... K>(std::index_sequence<K...>) {
    return (... + (static_cast<void>(K), std::invoke(fn)));
  }(std::make_index_sequence<N>());
}

/// Invoke the given object N times in an unrolled loop,
/// instantiating its operator() template with the loop index
/// for each invocation, and compute the sum of the results.
/// 
/// @details
/// Equivalent to (fn<0>() + fn<1>() + ... + fn<N-1>())
/// 
/// @tparam N
/// The number of inline invocations
template<size_t N, class Fn>
  requires (N != 0) && requires {
    std::declval<Fn>().template operator()<N>() +
    std::declval<Fn>().template operator()<N>();
  }
constexpr auto inline_sum(Fn&& fn) {
  return [&]<size_t... K>(std::index_sequence<K...>) {
    return (... + fn.template operator()<K>());
  }(std::make_index_sequence<N>());
}

/// Invoke the given object N times in an unrolled loop
/// and compute the logical conjunction of the results.
/// 
/// @details
/// Equivalent to (fn() && fn() && ... && fn()), including
/// short-circuiting.
/// 
/// @tparam N
/// The number of inline invocations
template<size_t N, std::invocable Fn>
  requires std::convertible_to<std::invoke_result_t<Fn>, bool>
constexpr bool inline_all(Fn&& fn) {
  return [&]<size_t... K>(std::index_sequence<K...>) {
    return (... && (static_cast<void>(K), std::invoke(fn)));
  }(std::make_index_sequence<N>());
}

/// Invoke the given object N times in an unrolled loop,
/// instantiating its operator() template with the loop index
/// for each invocation, and compute the logical conjunction
/// of the results.
/// 
/// @details
/// Equivalent to (fn<0>() && fn<1>() && ... && fn<N-1>()),
/// including short-circuiting.
/// 
/// @tparam N
/// The number of inline invocations
template<size_t N, class Fn>
  requires requires {
    std::declval<Fn>().template operator()<N>() &&
    std::declval<Fn>().template operator()<N>();
  }
constexpr bool inline_all(Fn&& fn) {
  return [&]<size_t... K>(std::index_sequence<K...>) {
    return (... && fn.template operator()<K>());
  }(std::make_index_sequence<N>());
}

/// Invoke the given object N times in an unrolled loop
/// and compute the logical disjunction of the results.
/// 
/// @details
/// Equivalent to (fn() || fn() || ... || fn()), including
/// short-circuiting.
/// 
/// @tparam N
/// The number of inline invocations
template<size_t N, std::invocable Fn>
  requires std::convertible_to<std::invoke_result_t<Fn>, bool>
constexpr bool inline_any(Fn&& fn) {
  return [&]<size_t... K>(std::index_sequence<K...>) {
    return (... || (static_cast<void>(K), std::invoke(fn)));
  }(std::make_index_sequence<N>());
}

/// Invoke the given object N times in an unrolled loop,
/// instantiating its operator() template with the loop index
/// for each invocation, and compute the logical disjunction
/// of the results.
/// 
/// @details
/// Equivalent to (fn<0>() || fn<1>() || ... || fn<N-1>()),
/// including short-circuiting.
/// 
/// @tparam N
/// The number of inline invocations
template<size_t N, class Fn>
  requires requires {
    std::declval<Fn>().template operator()<N>() ||
    std::declval<Fn>().template operator()<N>();
  }
constexpr bool inline_any(Fn&& fn) {
  return [&]<size_t... K>(std::index_sequence<K...>) {
    return (... || fn.template operator()<K>());
  }(std::make_index_sequence<N>());
}

}
