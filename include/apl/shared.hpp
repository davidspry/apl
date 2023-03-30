//
// Created by David Spry on 22/3/2023.
//

#pragma once

#include <atomic>
#include <concepts>
#include <apl/detail/notify_detail.hpp>
#include <apl/detail/shared_detail.hpp>
#include <apl/notify.hpp>
#include <type_traits>

namespace apl {

namespace impl {

//! MARK: - Implementation

template<detail::state_type state,
        detail::storage_type<typename state::value_type> storage_type> requires
std::is_default_constructible<typename state::value_type>::value and
std::is_move_constructible<typename state::value_type>::value or
std::is_copy_constructible<typename state::value_type>::value
class shared final {
    inline static storage_type m_storage{};

public:
    enum class notifications {
        will_set,
        did_set
    };

    //! Read the underlying value.
    constexpr static auto get()
    requires (std::same_as<typename state::atomic, std::false_type>) {
        return m_storage.get();
    }

    //! Read the underlying value.
    //! @param memory_order The memory-order to use when setting the underlying `std::atomic`.
    constexpr static auto get(const std::memory_order memory_order = std::memory_order::seq_cst)
    requires (std::same_as<typename state::atomic, std::true_type>) {
        return m_storage.get(memory_order);
    }

    //! Set the underlying value.
    constexpr static auto set(const state::value_type value)
    requires (std::same_as<typename state::atomic, std::false_type>) {
        m_storage.set(std::move(value));
    }

    //! Set the underlying value.
    //! @param value The new value to be set.
    //! @param memory_order The memory-order to use when setting the underlying `std::atomic`.
    constexpr static auto set(const state::value_type value,
                              const std::memory_order memory_order = std::memory_order::seq_cst)
    requires (std::same_as<typename state::atomic, std::true_type>) {
        m_storage.set(std::move(value), memory_order);
    }

    //! Set the underlying value and emit accompanying
    //! will-set & did-set notifications to applicable types.
    //! @returns A function to which notifiable objects
    //! should be supplied as parameters.
    constexpr static auto set_notify(const state::value_type value) {
        return [value = std::move(value)](detail::enum_notifiable<notifications> auto&& ... targets) {
            constexpr static auto will_set = [](auto& notifiable) {
                if constexpr (detail::enum_notifiable<decltype(notifiable), notifications>) {
                    notify<shared::notifications::will_set>(notifiable);
                }
            };

            constexpr static auto did_set = [](auto& notifiable) {
                if constexpr (detail::enum_notifiable<decltype(notifiable), notifications>) {
                    notify<shared::notifications::did_set>(notifiable);
                }
            };

            (will_set(targets), ...);
            impl::shared<state, storage_type>::set(value);
            (did_set(targets), ...);
        };
    }

    //! MARK: - Notifications

    template<auto notification>
    consteval static auto is_notification() {
        return std::same_as<notifications, decltype(notification)>;
    }

    template<auto notification>
    consteval static auto will_set() {
        return is_notification<notification>() &&
               static_cast<notifications>(notification) == notifications::will_set;
    }

    template<auto notification>
    consteval static auto did_set() {
        return is_notification<notification>() &&
               static_cast<notifications>(notification) == notifications::did_set;
    }
};

//! MARK: - Storage

template<typename value_type>
class storage final {
    value_type m_value{};

public:
    auto get() const {
        return m_value;
    }

    auto set(const value_type value) {
        if constexpr (std::is_move_assignable<value_type>::value) {
            m_value = std::move(value);
        } else {
            m_value = value;
        }
    }
};

template<typename value_type>
class atomic_storage final {
    std::atomic<value_type> m_value{};

public:
    auto get(const std::memory_order memory_order = std::memory_order::seq_cst) const {
        return m_value.load(memory_order);
    }

    auto set(const value_type value,
             const std::memory_order memory_order = std::memory_order::seq_cst) {
        if constexpr (std::is_move_assignable<value_type>::value) {
            m_value.store(std::move(value), memory_order);
        } else {
            m_value.store(value, memory_order);
        }
    }
};

}

//! MARK: - Public Type

//! A static-value described and uniquely-identified by the given state-type.
//! @note If the given state-type is marked as 'atomic', then the underlying
//! value will be used to specialise a `std::atomic` object.
template<detail::state_type state>
using shared = impl::shared<
        state,
        std::conditional_t<
                std::same_as<typename state::atomic, std::true_type>,
                impl::atomic_storage<typename state::value_type>,
                impl::storage<typename state::value_type>
        >
>;

}
