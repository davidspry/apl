///     ,.  .-,--. .
///    / |   '|__/ |
///   /~~|-. .|    |
/// ,'   `-' `'    `--'

#pragma once

#include <apl/meta/platform.h>

#if APL_ARCH_X86
  #if APL_COMPILER_MSVC
    #include <intrin.h>
  #else
    #include <immintrin.h>
  #endif
#endif

#if APL_ARCH_ARM
  #include <arm_acle.h>
#endif

#include <cstddef>
#include <functional>
#include <thread>

namespace apl::meta
{

/// @brief
/// Issue a CPU relaxation hint
/// 
/// @remarks
/// This is intended to relax resource usage during a spin-wait loop
/// 
/// @note
/// This function is defined for x86-64/32 and ARM64/32 architectures. It's a no-op otherwise.
APL_ALWAYS_INLINE inline void pause() noexcept {
  #if APL_ARCH_X86
    _mm_pause();
  #elif APL_ARCH_ARM
    #if defined(__ARM_ACLE) && (__ARM_ACLE >= 200)
      __yield();
    #else
      asm volatile("yield" ::: "memory");
    #endif
  #endif
}

/// @brief
/// Spin-wait with progressive back-off until the given predicate becomes true
/// 
/// @tparam T1
/// Stage 1: Spins without pausing
/// 
/// @tparam T2
/// Stage 2: Spins interleaved with light-weight pauses
/// 
/// @tparam T3
/// Stage 3: Spins interleaved with light-weight pauses, terminated by a scheduler yield.
/// Stage 3 repeats until the predicate becomes true.
/// 
/// @tparam Fn
/// Predicate function
template<size_t T1, size_t T2, size_t T3, class Fn>
  requires std::is_invocable_r_v<bool, Fn>
auto wait_with_spin_backoff(Fn&& fn) {
  for (size_t i = 0; i < T1; ++i) {
    if (std::invoke(fn))
      return;
  }

  for (size_t i = 0; i < T2; ++i) {
    if (std::invoke(fn))
      return;

    pause();
  }

  while (true) {
    for (size_t i = 0; i < T3; ++i) {
      if (std::invoke(fn))
        return;

      pause();
    }

    std::this_thread::yield();
  }
}

}
