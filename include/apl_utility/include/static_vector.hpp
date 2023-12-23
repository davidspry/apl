//! apl
//! David Spry
//! Created on 1/12/2023

#pragma once

#include <memory_resource>
#include <ranges>
#include <span>
#include <vector>

namespace apl {

//! @brief A specialisation of std::vector using statically-allocated storage.

template<class T, std::size_t SIZE>
class static_vector final: std::pmr::vector<T> {
  using storage_t = std::aligned_storage_t<sizeof(T), alignof(T)>;
  std::array<storage_t, SIZE> m_memory;
  std::pmr::monotonic_buffer_resource m_resource{m_memory.data(), sizeof(m_memory), std::pmr::null_memory_resource()};

public:
  static_vector():
    std::pmr::vector<T>(std::pmr::polymorphic_allocator<T>(&m_resource)) {
    std::pmr::vector<T>::reserve(SIZE);
  }

  static_vector(static_vector&&) = default;

  template<std::size_t N> requires (N <= SIZE)
  explicit static_vector(std::span<const T, N> span): static_vector() {
    std::ranges::copy(span, std::back_inserter(*this));
  }

  explicit static_vector(std::span<const T> span): static_vector() {
    std::copy(span.begin(), span.begin() + std::min(span.size(), SIZE), std::back_inserter(*this));
  }

  // =============================================================================
  using std::pmr::vector<T>::begin;
  using std::pmr::vector<T>::capacity;
  using std::pmr::vector<T>::data;
  using std::pmr::vector<T>::emplace_back;
  using std::pmr::vector<T>::empty;
  using std::pmr::vector<T>::end;
  using std::pmr::vector<T>::erase;
  using std::pmr::vector<T>::max_size;
  using std::pmr::vector<T>::operator[];
  using std::pmr::vector<T>::push_back;
  using std::pmr::vector<T>::rbegin;
  using std::pmr::vector<T>::rend;
  using std::pmr::vector<T>::resize;
  using std::pmr::vector<T>::size;
  using typename std::pmr::vector<T>::value_type;
};

}
