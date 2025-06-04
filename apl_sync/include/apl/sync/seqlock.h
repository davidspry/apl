///     ,.  .-,--. .
///    / |   '|__/ |
///   /~~|-. .|    |
/// ,'   `-' `'    `--'

#pragma once

#include <apl/meta/spin_wait.h>

#include <atomic>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace apl::sync
{

/// An object protected by a seqlock supporting lock-free SPMC concurrency
/// 
/// @details
/// Writing is wait-free. Writing can interrupt reading.
/// 
/// @pre
/// The protected value type is required to be trivially copyable
template<class T> requires std::is_trivially_copyable_v<T>
class Seqlock final {
  alignas(128) T m_value;
  alignas(128) std::atomic<size_t> m_seq;

public:
  Seqlock() requires std::is_default_constructible_v<T> = default;
  Seqlock(T initial_value): m_value(std::move(initial_value)) {}
  Seqlock(Seqlock const&) = delete;
  Seqlock(Seqlock&&) noexcept = delete;

  /// Obtain a copy of the protected value
  /// 
  /// @details
  /// This function will spin (with progressive back-off)
  /// until the value is successfully copied.
  auto load() noexcept -> T;

  /// Try to obtain a copy of the protected value
  /// 
  /// @param value
  /// The object into which the protected value should be copied
  /// 
  /// @returns
  /// True if the read was successful; false otherwise
  bool try_load(T& value) noexcept;

  /// Store the given value
  void store(T value) noexcept;

  auto operator=(Seqlock const&) = delete;
  auto operator=(Seqlock&&) noexcept = delete;
};

/// ,-_/ ,-,-,-.  .-,--. .
/// '  | `,| | |   '|__/ |
/// .- |   | ; | . .|    |
/// `--'   '   `-' `'    `--'

template<class T> requires std::is_trivially_copyable_v<T>
auto Seqlock<T>::load() noexcept -> T {
  alignas(T) std::byte buf[sizeof(T)];

  apl::meta::wait_with_spin_backoff<8, 64, 512>([&, this]() {
    return try_load(*reinterpret_cast<T*>(buf));
  });

  return *reinterpret_cast<T*>(buf);
}

template<class T> requires std::is_trivially_copyable_v<T>
bool Seqlock<T>::try_load(T& value) noexcept {
  auto seq1 = m_seq.load(std::memory_order::acquire);

  if (seq1 & 1)
    return false;

  std::memcpy(&value, &m_value, sizeof(T));
  const auto seq2 = m_seq.load(std::memory_order::acquire);

  return seq1 == seq2;
}

template<class T> requires std::is_trivially_copyable_v<T>
void Seqlock<T>::store(T value) noexcept {
  m_seq.fetch_add(1, std::memory_order::release);
  std::memcpy(&m_value, &value, sizeof(T));
  m_seq.fetch_add(1, std::memory_order::release);
}

}
