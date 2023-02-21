//
// Created by David Spry on 20/1/2023.
//

#pragma once

#include <atomic>
#include <concepts>
#include <cstdint>
#include <optional>
#include <type_traits>

namespace apl {

//! A thread-safe object-wrapper based on the 'seqlock' spin-lock algorithm.

template<typename value_type>
class seqlock final {
public:
    constexpr seqlock() = default;
    constexpr explicit seqlock(const value_type value): m_value(value) {}

    [[nodiscard]] constexpr auto load() -> value_type {
        while (true) {
            const auto sequence_number1 = m_sequence_number.load(std::memory_order::acquire);
            std::atomic_thread_fence(std::memory_order::acquire);
            const auto value = m_value;
            const auto sequence_number2 = m_sequence_number.load(std::memory_order::relaxed);
            if ((sequence_number1 & 1) == 0 && sequence_number1 == sequence_number2) {
                return value;
            }
        }
    }

    [[nodiscard]] constexpr auto try_load() -> std::optional<value_type> {
        const auto sequence_number1 = m_sequence_number.load(std::memory_order::acquire);
        std::atomic_thread_fence(std::memory_order::acquire);
        const auto value = m_value;
        const auto sequence_number2 = m_sequence_number.load(std::memory_order::relaxed);
        if ((sequence_number1 & 1) == 0 && sequence_number1 == sequence_number2) {
            return value;
        } else {
            return std::nullopt;
        }
    }

    constexpr auto store(const value_type& value) {
        m_sequence_number.fetch_add(1, std::memory_order::relaxed);
        std::atomic_thread_fence(std::memory_order::release);
        std::memcpy(&m_value, &value, sizeof(value_type));
        m_sequence_number.fetch_add(1, std::memory_order::release);
    }

    constexpr auto store(value_type&& value) requires std::is_move_assignable<value_type>::value {
        m_sequence_number.fetch_add(1, std::memory_order::relaxed);
        std::atomic_thread_fence(std::memory_order::release);
        m_value = std::move(value);
        m_sequence_number.fetch_add(1, std::memory_order::release);
    }

private:
    value_type m_value{};

    std::atomic<std::uint64_t> m_sequence_number{};
};

}
