//
// Created by David Spry on 2/3/2023.
//

#pragma once

#include <memory>

namespace apl {

//! A lock-free, thread-safe object-wrapper. The locking-mechanism
//! supports multiple writers but only a single reader.
//!
//! Reading is wait-free. Writers wait for readers.
//!
//! @see "Thread synchronisation in real-time audio processing with RCU", Timur Doumler, ADC 22

template<typename value_type>
class cexlock {
    std::unique_ptr<value_type> m_value{};
    std::atomic<value_type*> m_value_ptr{nullptr};

    class reader final {
        std::atomic<value_type*>& m_value_ptr_ref;
        value_type* const m_value_ptr;

    public:
        constexpr explicit reader(std::atomic<value_type*>& value_ptr_ref):
                m_value_ptr_ref(value_ptr_ref),
                m_value_ptr(m_value_ptr_ref.exchange(nullptr)) {}

        constexpr ~reader() {
            m_value_ptr_ref.exchange(m_value_ptr);
        }

        constexpr auto is_null() const {
            return m_value_ptr == nullptr;
        }

        constexpr auto operator*() const -> value_type& {
            return *m_value_ptr;
        }

        constexpr auto operator->() const -> value_type* {
            return m_value_ptr;
        }
    };

public:
    constexpr cexlock() requires std::is_default_constructible<value_type>::value:
            m_value(std::make_unique<value_type>()),
            m_value_ptr(m_value.get()) {}

    template<typename ...ctor_parameters>
    constexpr explicit cexlock(ctor_parameters&& ... parameters):
            m_value(std::make_unique<value_type>(std::forward<ctor_parameters>(parameters)...)),
            m_value_ptr(m_value.get()) {}

    constexpr cexlock(const cexlock&) = delete;
    constexpr cexlock(const cexlock&&) = delete;
    constexpr auto operator=(const cexlock&) = delete;
    constexpr auto operator=(const cexlock&&) noexcept = delete;

    //! Temporarily obtain exclusive access in order to copy
    //! the underlying value.
    constexpr auto copy_read() -> value_type requires std::is_trivially_copyable<value_type>::value {
        const auto locked = lock_read();
        value_type value;
        std::memcpy(&value, m_value.get(), sizeof(value_type));
        return value;
    }

    //! Obtain exclusive read-only access to the underlying value.
    //! The exclusivity will be revoked automatically when the
    //! returned value is destroyed.
    constexpr auto lock_read() -> reader {
        return reader{m_value_ptr};
    }

    //! Update the underlying value.
    //! @note The writer will spin until the write operation can be performed.
    constexpr auto write(const value_type new_value) requires std::move_constructible<value_type> {
        auto new_value_ptr = std::make_unique<value_type>(std::move(new_value));
        auto current_value = m_value.get();
        while (!m_value_ptr.compare_exchange_weak(current_value, new_value_ptr.get())) {
            current_value = m_value.get();
        }

        m_value = std::move(new_value_ptr);
    }
};

}
