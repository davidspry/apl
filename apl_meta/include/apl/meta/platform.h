///     ,.  .-,--. .
///    / |   '|__/ |
///   /~~|-. .|    |
/// ,'   `-' `'    `--'

#pragma once

/// Architecture
#if defined(_M_X64) || defined(__x86_64__) || defined(__amd64__)
  #define APL_ARCH_X86_64 1
  #define APL_ARCH_X86 1
#elif defined(_M_IX86) || defined(__i386__)
  #define APL_ARCH_X86_32 1
  #define APL_ARCH_X86 1
#elif defined(_M_ARM64) || defined(__aarch64__)
  #define APL_ARCH_ARM64 1
  #define APL_ARCH_ARM 1
#elif defined(_M_ARM) || defined(__arm__)
  #define APL_ARCH_ARM32 1
  #define APL_ARCH_ARM 1
#else
  #warning "Unsupported architecture"
#endif

/// Compiler
#if defined(__INTEL_LLVM_COMPILER)
  #define APL_COMPILER_INTEL 1
  #define APL_COMPILER_INTEL_LLVM 1
#elif defined(__clang__)
  #define APL_COMPILER_CLANG 1
  #if defined(__apple_build_version__)
    #define APL_COMPILER_APPLE_CLANG 1
  #endif
  #if defined(__MINGW32__) || defined(__MINGW64__)
    #define APL_COMPILER_MINGW 1
  #endif
#elif defined(_MSC_VER)
  #define APL_COMPILER_MSVC 1
#elif defined(__INTEL_COMPILER)
  #define APL_COMPILER_INTEL 1
#elif defined(__GNUC__)
  #define APL_COMPILER_GCC 1
  #if defined(__MINGW32__) || defined(__MINGW64__)
    #define APL_COMPILER_MINGW 1
  #endif
#else
  #warning "Unsupported compiler"
#endif

/// Attributes
#if APL_COMPILER_CLANG || APL_COMPILER_GCC
  #define APL_ALWAYS_INLINE __attribute__((always_inline))
#elif APL_COMPILER_MSVC
  #define APL_ALWAYS_INLINE __forceinline
#else
  #define APL_ALWAYS_INLINE
#endif
