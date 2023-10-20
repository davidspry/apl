//! apl
//! David Spry
//! Created on 16/10/2023

#pragma once

#include <type_traits>

namespace apl {

//! Apply arguments to a subsequently-given function.
//! @code apply(args...)(f) -> f(args...)
static constexpr auto apply = []<class ...T>(T&& ... arguments) {
  return [&arguments...]<class F>(F&& function) -> decltype(auto) {
    return std::invoke(
      std::forward<F>(function),
      std::forward<T>(arguments)...
    );
  };
};

//! Apply subsequently-given arguments to a function.
//! @code apply_to(f)(args...) -> f(args...)
static constexpr auto apply_to = []<class F>(F&& function) {
  return [function = std::forward<F>(function)]<class ...T>(T&& ... arguments) -> decltype(auto)
  requires(std::is_invocable_v<F, T...>) {
    return std::invoke(std::forward<const F>(function), std::forward<T>(arguments)...);
  };
};

//! Apply two sets of arguments to a function
//! @code partial(f)(args1...)(args2...) -> f(args1..., args2...)
static constexpr auto partial = []<class F>(F&& function) {
  return [&function]<class... T>(T&& ...applied_arguments) {
    return [&applied_arguments..., &function]<class ...U>(U&& ... invoked_arguments) {
      return apl::apply_to(std::forward<F>(function))(
        std::forward<T>(applied_arguments)...,
        std::forward<U>(invoked_arguments)...
      );
    };
  };
};

//! Apply two sets of arguments, in swapped order, to a function.
//! @code partial_swap(f)(args1...)(args2...) -> f(args2..., args1...)
static constexpr auto partial_swap = []<class F>(F&& function) {
  return [&function]<class... T>(T&& ...applied_arguments) {
    return [&applied_arguments..., &function]<class ...U>(U&& ... invoked_arguments) {
      return apl::apply_to(std::forward<F>(function))(
        std::forward<U>(invoked_arguments)...,
        std::forward<T>(applied_arguments)...
      );
    };
  };
};

//! Apply two sets of arguments to a function.
//! @code partial_apply(args1...)(f)(args2...) -> f(args1..., args2...)
static constexpr auto partial_apply = []<class ...T>(T&& ... applied_arguments) {
  return [&applied_arguments...]<class F>(F&& function) -> decltype(auto) {
    return apl::partial(std::forward<F>(function))(std::forward<T>(applied_arguments)...);
  };
};

//! Apply two sets of arguments, in swapped order, to a function.
//! @code partial_apply_swap(args1...)(f)(args2...) -> f(args2..., args1...)
static constexpr auto partial_apply_swap = []<class ...T>(T&& ... applied_arguments) {
  return [&applied_arguments...]<class F>(F&& function) -> decltype(auto) {
    return apl::partial_swap(std::forward<F>(function))(std::forward<T>(applied_arguments)...);
  };
};

//! Compose two functions
//! @code compose(f, g)(args...) -> f(g(args...))
static constexpr auto compose = []<class F, class G>(F&& function1, G&& function2) {
  return [
    fn1 = std::forward<F>(function1),
    fn2 = std::forward<G>(function2)
  ]<class ...T>(T&& ... arguments) requires
  (std::is_invocable_v<F, std::invoke_result_t<G, T...>> &&
   std::is_invocable_v<G, T...>) {
    return std::invoke(fn1, std::invoke(fn2, std::forward<T>(arguments)...));
  };
};

}
