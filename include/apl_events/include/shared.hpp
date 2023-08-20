//
// Created by David Spry on 22/3/2023.
//

#pragma once

#include <atomic>
#include <concepts>
#include <type_traits>

#include "notify.hpp"
#include "detail/notify_detail.hpp"
#include "detail/shared_detail.hpp"

namespace apl::impl {

template<
  detail::state_type T,
  detail::storage_type<typename T::value_type> storage_type
> requires std::is_default_constructible<typename T::value_type>::value and
           (std::is_move_constructible<typename T::value_type>::value or
            std::is_copy_constructible<typename T::value_type>::value)
class shared final {
  inline static storage_type m_storage{};
  static constexpr auto atomic_storage = std::same_as<typename T::atomic, std::true_type>;
  static constexpr auto non_atomic_storage = std::same_as<typename T::atomic, std::false_type>;

public:
  enum class notifications {
    will_set,
    did_set
  };

  // MARK: - Storage Interface

  //! Read the underlying value.
  static constexpr auto get() -> T::value_type requires non_atomic_storage {
    return m_storage.get();
  }

  //! Set the underlying value.
  static constexpr void set(const T::value_type value) requires non_atomic_storage {
    m_storage.set(std::move(value));
  }

  //! Set the underlying value and emit accompanying will-set & did-set notifications to applicable types.
  //! @returns A function, to which notifiable objects should be supplied as parameters.
  static constexpr auto set_notify(T::value_type value) requires non_atomic_storage {
    return set_notify_impl(std::move(value));
  }

  // MARK: - Atomic Storage Interface

  //! Read the underlying value.
  //! @param memory_order The memory-order to use when setting the underlying `std::atomic`.
  static constexpr auto get(
    const std::memory_order memory_order = std::memory_order::seq_cst
  ) -> T::value_type requires atomic_storage {
    return m_storage.get(memory_order);
  }

  //! Set the underlying value.
  //! @param value The new value to be set.
  //! @param memory_order The memory-order to use when setting the underlying `std::atomic`.
  static constexpr void set(
    const T::value_type value,
    const std::memory_order memory_order = std::memory_order::seq_cst
  ) requires atomic_storage {
    m_storage.set(std::move(value), memory_order);
  }

  //! Set the underlying value and emit accompanying will-set & did-set notifications to applicable types.
  //! @param value The new value to be set.
  //! @param memory_order The memory-order to use when setting the underlying `std::atomic`.
  //! @returns A function, to which notifiable objects should be supplied as parameters.
  static constexpr auto set_notify(
    T::value_type value,
    std::memory_order memory_order = std::memory_order::seq_cst
  ) requires atomic_storage {
    return set_notify_impl(std::move(value), memory_order);
  }

  // MARK: - Notifications Interface

  template<auto notification> static constexpr bool is_notification() {
    return std::same_as<notifications, decltype(notification)>;
  }

  template<auto notification> static constexpr bool will_set() {
    return is_notification<notification>() &&
           static_cast<notifications>(notification) == notifications::will_set;
  }

  template<auto notification> static constexpr bool did_set() {
    return is_notification<notification>() &&
           static_cast<notifications>(notification) == notifications::did_set;
  }

private:
  // MARK: - Set Notify

  static constexpr auto set_notify_impl(
    const T::value_type value,
    const std::memory_order memory_order = std::memory_order::seq_cst
  ) {
    static constexpr auto notify_will_set = [](auto& notifiable) {
      if constexpr (detail::enum_notifiable<decltype(notifiable), notifications>) {
        notify<shared::notifications::will_set>(notifiable);
      }
    };

    static constexpr auto notify_did_set = [](auto& notifiable) {
      if constexpr (detail::enum_notifiable<decltype(notifiable), notifications>) {
        notify<shared::notifications::did_set>(notifiable);
      }
    };

    return [value = std::move(value), memory_order](detail::enum_notifiable<notifications> auto&& ... targets) {
      (notify_will_set(targets), ...);

      if constexpr (atomic_storage) {
        impl::shared<T, storage_type>::set(value, memory_order);
      } else {
        impl::shared<T, storage_type>::set(value);
      }

      (notify_did_set(targets), ...);
    };
  }
};

// MARK: - Storage
// ==========================================================================

template<typename T>
class storage final {
  T m_value{};

public:
  auto get() const { return m_value; }
  auto set(T value) { m_value = value; }
};

template<typename T>
class atomic_storage final {
  std::atomic<T> m_value{};

public:
  auto get(const std::memory_order memory_order = std::memory_order::seq_cst) const {
    return m_value.load(memory_order);
  }

  auto set(T value, const std::memory_order memory_order = std::memory_order::seq_cst) {
    m_value.store(std::move(value), memory_order);
  }
};

template<detail::state_type T>
using storage_t = std::conditional_t<
  std::same_as<typename T::atomic, std::true_type>,
  impl::atomic_storage<typename T::value_type>,
  impl::storage<typename T::value_type>
>;

}

// MARK: - Alias
// ==========================================================================

namespace apl {

//! A static-value described and uniquely-identified by the given state-type.
template<detail::state_type T> using shared = impl::shared<T, impl::storage_t<T>>;

}
