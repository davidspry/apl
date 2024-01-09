//! apl
//! David Spry
//! Created on 23/12/2023

#pragma once

#include <bit>
#include <cstddef>
#include <type_traits>
#include <queue>

#include <apl_utility/include/mod.hpp>
#include <apl_utility/include/inplace_vector.hpp>

namespace apl {

//! @brief A circular FIFO with a fixed capacity, using in-built storage.
//! @tparam T Value-type
//! @tparam N Capacity

template<class T, std::size_t N> requires std::is_trivially_copyable_v<T>
class inplace_fifo final {
  alignas(T) std::array<std::byte, sizeof(T) * N> m_buffer{};
  apl::mod<N> m_head{0};
  std::size_t m_size{0};

public:
  inplace_fifo() = default;

  // ============================================================================
  // @formatter:off
  [[nodiscard]] auto size() const -> std::size_t                  { return m_size; }
  [[nodiscard]] auto full() const -> bool                         { return m_size == N; }
  [[nodiscard]] auto empty() const -> bool                        { return m_size == 0; }
  [[nodiscard]] static consteval auto capacity() -> std::size_t   { return N; }
  // @formatter:on

  // ============================================================================
  //! The item at the front of the queue.
  [[nodiscard]] auto front() -> T& {
    return *std::launder(std::bit_cast<T*>(&m_buffer[m_head.value() * sizeof(T)]));
  }

  //! The item at the front of the queue.
  [[nodiscard]] auto front() const -> const T& {
    return *std::launder(std::bit_cast<const T*>(&m_buffer[m_head.value() * sizeof(T)]));
  }

  //! Discard the frontmost item.
  //! @return Whether an element was discarded or not.
  void pop_front() {
    if (!empty()) {
      m_head.increment();
      m_size -= 1;
    }
  }

  // ============================================================================
  //! Enqueue a new item.
  //! @param item The item to be copied into the queue.
  void push_back(const T item) {
    const auto index = m_head.incremented_by(m_size).value();
    std::memcpy(&m_buffer[index * sizeof(T)], &item, sizeof(item));
    did_increase_size_by_count(1);
  }

  //! Enqueue multiple new items.
  //! @param span The array of items to be copied into the queue.
  void push_back(const std::span<const T> span) {
    const auto index = m_head.incremented_by(m_size).value();
    const auto count = capacity() - index;
    if (const auto size = span.size(); size <= capacity()) {
      std::memcpy(&m_buffer[index * sizeof(T)], &span[0], count * sizeof(T));
      std::memcpy(&m_buffer[0], &span[count], index * sizeof(T));
      did_increase_size_by_count(size);
    } else {
      const auto offset = size - capacity();
      std::memcpy(&m_buffer[index * sizeof(T)], &span[offset], count * sizeof(T));
      std::memcpy(&m_buffer[0], &span[offset + count], (capacity() - count) * sizeof(T));
      did_increase_size_by_count(capacity());
    }
  }

  // MARK: - Iterator
  // ============================================================================
  struct iterator {
    using value_type = T;
    using pointer = value_type*;
    using reference = value_type&;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    constexpr explicit iterator(const inplace_fifo* fifo, mod<N> index, std::size_t count):
      m_fifo(fifo),
      m_count(count),
      m_index(index) {
    }

    constexpr auto operator*() const -> const T& {
      return *std::launder(std::bit_cast<const T*>(&m_fifo->m_buffer[m_index.value() * sizeof(T)]));
    }

    constexpr auto operator!=(const iterator& other) const -> bool {
      return m_fifo != other.m_fifo || m_count != other.m_count || m_index != other.m_index;
    }

    constexpr auto& operator++() {
      m_index.increment();
      m_count -= 1;
      return *this;
    }

  private:
    const inplace_fifo* m_fifo;
    std::size_t m_count;
    mod<N> m_index;
  };

  // MARK: - Iterators
  // ============================================================================
  // @formatter:off
  [[nodiscard]] auto begin()       { return iterator(this, m_head, m_size); }
  [[nodiscard]] auto begin() const { return iterator(this, m_head, m_size); }
  [[nodiscard]] auto end()         { return iterator(this, m_head.incremented_by(m_size), 0); }
  [[nodiscard]] auto end() const   { return iterator(this, m_head.incremented_by(m_size), 0); }
  // @formatter:on

private:
  void did_increase_size_by_count(const std::size_t count) {
    if (m_size == N) { m_head.increment_by(count); }
    if (m_size != N) { m_size += count; }
  }
};

}
