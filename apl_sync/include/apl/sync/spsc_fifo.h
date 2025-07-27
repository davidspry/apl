///     ,.  .-,--. .
///    / |   '|__/ |
///   /~~|-. .|    |
/// ,'   `-' `'    `--'

#pragma once

#include <atomic>
#include <bit>
#include <memory>
#include <new>
#include <optional>
#include <type_traits>
#include <utility>

namespace apl::sync
{

/// A FIFO supporting lock-free SPSC concurrency
/// 
/// @details
/// Reading is wait-free. Writing will fail if the FIFO is full.
/// 
/// @tparam T
/// The protected value type
/// 
/// @tparam N
/// The FIFO capacity, required to be a power of 2 greater than 1
template<class T, size_t N> requires (N != 1 && std::has_single_bit(N))
class SpscFifo {
  struct alignas(128) Box {
    alignas(T) std::byte buffer[sizeof(T)];
  };

  Box m_buffer[N]{};
  alignas(128) std::atomic<size_t> m_enqueue{0};
  alignas(128) std::atomic<size_t> m_dequeue{0};
  static consteval auto mask() { return N - 1; }

public:
  SpscFifo() = default;
  SpscFifo(SpscFifo&&) noexcept = delete;
  SpscFifo(const SpscFifo&) = delete;

  /// Indicate whether the FIFO is currently empty or not
  bool empty() const noexcept;

  /// Try to enqueue a new item
  /// 
  /// @returns
  /// True if the item was enqueued; false otherwise
  bool try_enqueue(T item) noexcept(std::is_nothrow_constructible_v<T, T&&>);

  /// Try to dequeue the next item
  /// 
  /// @returns
  /// An optional value, which will be nullopt if the FIFO is empty
  auto try_dequeue() noexcept(std::is_nothrow_destructible_v<T>) -> std::optional<T>;

  auto operator=(SpscFifo&&) noexcept = delete;
  auto operator=(const SpscFifo&) = delete;
};

/// ,-_/ ,-,-,-.  .-,--. .
/// '  | `,| | |   '|__/ |
/// .- |   | ; | . .|    |
/// `--'   '   `-' `'    `--'

template<class T, size_t N> requires (N != 1 && std::has_single_bit(N))
bool SpscFifo<T, N>::empty() const noexcept {
  return m_enqueue.load(std::memory_order::acquire) ==
         m_dequeue.load(std::memory_order::acquire);
}

template<class T, size_t N> requires (N != 1 && std::has_single_bit(N))
bool SpscFifo<T, N>::try_enqueue(T item) noexcept(std::is_nothrow_constructible_v<T, T&&>) {
  const auto enq = m_enqueue.load(std::memory_order::relaxed);
  const auto deq = m_dequeue.load(std::memory_order::acquire);

  if (enq - deq == N)
    return false;

  auto& box = m_buffer[enq & mask()];
  auto* buf = reinterpret_cast<T*>(box.buffer);
  std::construct_at(buf, std::move(item));
  m_enqueue.store(enq + 1, std::memory_order::release);

  return true;
}

template<class T, size_t N> requires (N != 1 && std::has_single_bit(N))
auto SpscFifo<T, N>::try_dequeue() noexcept(std::is_nothrow_destructible_v<T>) -> std::optional<T> {
  const auto enq = m_enqueue.load(std::memory_order::acquire);
  const auto deq = m_dequeue.load(std::memory_order::relaxed);

  if (enq == deq)
    return std::nullopt;

  auto& box = m_buffer[deq & mask()];
  auto* buf = std::launder(reinterpret_cast<T*>(box.buffer));
  auto item = std::move(*buf);
  std::destroy_at(buf);
  m_dequeue.store(deq + 1, std::memory_order::release);

  return item;
}

}
