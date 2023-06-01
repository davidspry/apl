//! David Spry
//! apl

#pragma once

#include <apl/detail/common.hpp>
#include <array>
#include <atomic>
#include <bit>
#include <cstddef>
#include <optional>

namespace apl {

//! A queue supporting concurrent use by a single writer and a single reader.

template<detail::assignable value_type, std::size_t CAPACITY> requires (std::has_single_bit(CAPACITY))
class swsr_queue {
    std::array<value_type, CAPACITY> m_array{};
    std::atomic<std::size_t> m_size{0};
    std::atomic<std::size_t> m_enqueue_index{0};
    std::atomic<std::size_t> m_dequeue_index{0};
    static constexpr std::size_t m_array_bounds_mask = CAPACITY - 1;

    constexpr auto is_full() const -> bool {
        return m_size.load(std::memory_order::acquire) == CAPACITY;
    }

public:
    constexpr swsr_queue() requires std::is_default_constructible<value_type>::value = default;

    constexpr auto is_empty() const -> bool {
        return m_size.load(std::memory_order::acquire) == 0;
    }

    constexpr auto push(const value_type value) -> bool {
        if (!is_full()) {
            m_size.fetch_add(1, std::memory_order::release);
            const auto index = m_enqueue_index.load(std::memory_order::acquire);
            m_array[index] = std::move(value);
            m_enqueue_index.store((index + 1) & m_array_bounds_mask, std::memory_order::release);
            return true;
        } else {
            return false;
        }
    }

    constexpr auto pop() -> std::optional<value_type> {
        if (!is_empty()) {
            m_size.fetch_sub(1, std::memory_order::release);
            const auto index = m_dequeue_index.load(std::memory_order::acquire);
            const auto value = m_array[index];
            m_dequeue_index.store((index + 1) & m_array_bounds_mask, std::memory_order::release);
            return value;
        } else {
            return std::nullopt;
        }
    }
};

}
