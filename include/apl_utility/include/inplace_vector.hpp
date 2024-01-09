//! apl
//! David Spry
//! Created on 1/12/2023

#pragma once

#include <algorithm>
#include <memory_resource>
#include <span>
#include <vector>

namespace apl {

//! @brief A specialisation of std::vector with a fixed capacity, using in-built storage.

template<class T, std::size_t N>
class inplace_vector final: std::pmr::vector<T> {
  alignas(T) std::array<std::byte, sizeof(T) * N> m_memory;
  std::pmr::monotonic_buffer_resource m_resource{m_memory.data(), sizeof(m_memory), std::pmr::null_memory_resource()};

public:
  inplace_vector():
    std::pmr::vector<T>(std::pmr::polymorphic_allocator<T>(&m_resource)) {
    std::pmr::vector<T>::reserve(N);
  }

  explicit inplace_vector(std::ranges::sized_range auto&& range): inplace_vector() {
    std::copy_n(std::ranges::begin(range), std::min(std::ranges::size(range), N), std::back_inserter(*this));
  }

  inplace_vector(inplace_vector&&) = default;
  inplace_vector(const inplace_vector&) = default;
  inplace_vector& operator=(inplace_vector&&) noexcept = default;
  inplace_vector& operator=(const inplace_vector&) = default;

  // =============================================================================
  using std::pmr::vector<T>::at;
  using std::pmr::vector<T>::back;
  using std::pmr::vector<T>::begin;
  using std::pmr::vector<T>::capacity;
  using std::pmr::vector<T>::data;
  using std::pmr::vector<T>::emplace_back;
  using std::pmr::vector<T>::empty;
  using std::pmr::vector<T>::end;
  using std::pmr::vector<T>::erase;
  using std::pmr::vector<T>::max_size;
  using std::pmr::vector<T>::operator[];
  using std::pmr::vector<T>::pop_back;
  using std::pmr::vector<T>::push_back;
  using std::pmr::vector<T>::rbegin;
  using std::pmr::vector<T>::rend;
  using std::pmr::vector<T>::resize;
  using std::pmr::vector<T>::size;

  using typename std::pmr::vector<T>::const_iterator;
  using typename std::pmr::vector<T>::const_reference;
  using typename std::pmr::vector<T>::iterator;
  using typename std::pmr::vector<T>::reference;
  using typename std::pmr::vector<T>::size_type;
  using typename std::pmr::vector<T>::value_type;
};

}
