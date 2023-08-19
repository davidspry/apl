//
// Created by David Spry on 10/5/2023.
//

#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <functional>
#include <memory>
#include <type_traits>

namespace apl {

//! An object-store that uses double-buffering to support
//! concurrent, wait-free reading and writing, but only by
//! a single reader and a single writer.

template<typename T>
class double_buffer {
  std::array<T, 2> m_buffer;
  mutable std::atomic<bool> m_has_updates{false};
  mutable std::atomic<std::size_t> m_active_index{0};
  class writer;

public:
  double_buffer() requires std::is_default_constructible<T>::value = default;
  explicit double_buffer(const T initial): m_buffer({initial, initial}) {}
  double_buffer(double_buffer&&) noexcept = default;
  double_buffer(const double_buffer&) = delete;

  //! Obtain a reference to the buffer's read-position.
  auto get() -> T& {
    update_index();
    return m_buffer[m_active_index];
  }

  //! Obtain a const-reference to the buffer's read-position.
  auto get() const -> const T& {
    update_index();
    return m_buffer[m_active_index];
  }

  //! Set the value at the buffer's write-position.
  auto set(const T new_value) {
    m_has_updates.store(false, std::memory_order::release);
    m_buffer[write_index()] = std::move(new_value);
    m_has_updates.store(true, std::memory_order::relaxed);
  }

  //! Obtain a pointer to the buffer's write-position.
  //! The buffers will swap automatically when the
  //! returned pointer is destroyed.
  [[nodiscard]] auto write_ptr() -> writer {
    return {m_buffer.data() + write_index(), m_has_updates};
  }

private:
  //! Update the active index if the state of the write-object has changed.
  inline auto update_index() const {
    auto has_updates = true;
    if (m_has_updates.compare_exchange_weak(has_updates, false, std::memory_order::acquire)) {
      m_active_index.fetch_xor(1, std::memory_order::acquire);
    }
  }

  inline auto write_index() const -> std::size_t {
    return m_active_index.load(std::memory_order::acquire) xor 1;
  }
};

//! MARK: - Writer

template<typename T>
class double_buffer<T>::writer final {
  T* const m_value_ptr;
  std::atomic<bool>& m_update_flag;

public:
  writer(T* const value_ptr, std::atomic<bool>& update_flag):
    m_value_ptr(value_ptr),
    m_update_flag(update_flag) {
    m_update_flag.store(false, std::memory_order::release);
  }

  ~writer() {
    m_update_flag.store(true, std::memory_order::relaxed);
  }

  auto operator->() -> T* {
    return m_value_ptr;
  }

  auto operator*() -> T& {
    return *m_value_ptr;
  }
};

}
