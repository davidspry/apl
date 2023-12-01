//! apl
//! David Spry
//! Created on 1/12/2023

#pragma once

#include <memory_resource>
#include <vector>

namespace apl {

//! @brief A specialisation of std::vector using statically-allocated storage.

template<class T, std::size_t SIZE>
class static_vector final: std::pmr::vector<T> {
  std::array<T, SIZE> m_memory;
  std::pmr::monotonic_buffer_resource m_resource{m_memory.data(), sizeof(m_memory), std::pmr::null_memory_resource()};

public:
  static_vector():
    std::pmr::vector<T>(std::pmr::polymorphic_allocator<T>(&m_resource)) {
    std::pmr::vector<T>::reserve(SIZE);
  }

  // =============================================================================
  using std::pmr::vector<T>::begin;
  using std::pmr::vector<T>::capacity;
  using std::pmr::vector<T>::data;
  using std::pmr::vector<T>::emplace_back;
  using std::pmr::vector<T>::end;
  using std::pmr::vector<T>::max_size;
  using std::pmr::vector<T>::push_back;
  using std::pmr::vector<T>::rbegin;
  using std::pmr::vector<T>::rend;
  using std::pmr::vector<T>::resize;
  using std::pmr::vector<T>::size;
};

}
