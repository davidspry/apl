//
// Created by David Spry on 28/1/2023.
//

#pragma once

#include <apl/types/detail/common.hpp>
#include <array>
#include <atomic>
#include <bit>
#include <concepts>
#include <optional>

namespace apl {

template<typename value_type>
concept assignable = std::is_copy_assignable<value_type>::value ||
                     std::is_move_assignable<value_type>::value;

template<assignable value_type, std::size_t CAPACITY> requires (std::has_single_bit(CAPACITY))
class mwmr_queue {
    template<typename underlying_type>
    struct occupiable {
        std::atomic_flag is_occupied{false};
        std::optional<underlying_type> m_value{std::nullopt};
    };

    std::array<occupiable<value_type>, CAPACITY> m_array;
    std::atomic<std::size_t> m_enqueue_index{0};
    std::atomic<std::size_t> m_dequeue_index{0};
    const std::size_t m_array_bounds_mask = CAPACITY - 1;

public:
    constexpr mwmr_queue() = default;

    template<detail::decays_to<value_type> value_type_instance>
    constexpr auto push(value_type_instance&& value) -> bool {
        while (true) {
            auto enqueue_index = m_enqueue_index.load(std::memory_order::relaxed);
            auto& item = m_array[enqueue_index];
            const auto is_occupied = item.is_occupied.test(std::memory_order::acquire);

            if (!is_occupied) {
                if (m_enqueue_index.compare_exchange_weak(enqueue_index,
                                                          (enqueue_index + 1) & m_array_bounds_mask,
                                                          std::memory_order::relaxed)) {
                    item.m_value = std::forward<value_type_instance>(value);
                    item.is_occupied.test_and_set();
                    return true;
                }
            } else {
                return false;
            }
        }
    }

    constexpr auto pop() -> std::optional<value_type> {
        while (true) {
            auto dequeue_index = m_dequeue_index.load(std::memory_order::relaxed);
            auto& item = m_array[dequeue_index];
            const auto is_occupied = item.is_occupied.test(std::memory_order::acquire);

            if (is_occupied) {
                if (m_dequeue_index.compare_exchange_weak(dequeue_index,
                                                          (dequeue_index + 1) & m_array_bounds_mask,
                                                          std::memory_order::relaxed)) {
                    const auto value = std::move(item.m_value);
                    item.is_occupied.clear(std::memory_order::release);
                    return value;
                }
            } else {
                return std::nullopt;
            }
        }
    }
};

}
