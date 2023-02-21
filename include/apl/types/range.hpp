//
// Created by David Spry on 25/1/2023.
//

#pragma once

#include <algorithm>
#include <apl/types/detail/range_detail.hpp>
#include <concepts>
#include <fmt/format.h>
#include <iterator>

namespace apl::impl {

//! MARK: Implementation

template<std::int64_t LOWER_BOUND,
         std::int64_t UPPER_BOUND,
         std::uint64_t DELTA_VALUE,
         detail::iteration_direction iteration_direction> requires (UPPER_BOUND > LOWER_BOUND && DELTA_VALUE > 0)
struct range {
    //! MARK: Public Interface

    constexpr static auto size() -> std::size_t { return UPPER_BOUND - LOWER_BOUND; }
    constexpr static auto lower_bound() { return LOWER_BOUND; }
    constexpr static auto upper_bound() { return UPPER_BOUND; }

    constexpr static auto contains(std::integral auto value) -> bool {
        return LOWER_BOUND <= value && value < UPPER_BOUND;
    }

    //! MARK: Range-builder Aliases

    template<std::uint64_t SUFFIX_SIZE> requires (SUFFIX_SIZE < size())
    using drop_suffix = range<LOWER_BOUND,
                              UPPER_BOUND - static_cast<std::int64_t>(SUFFIX_SIZE),
                              DELTA_VALUE,
                              iteration_direction>;

    template<std::uint64_t PREFIX_SIZE> requires (PREFIX_SIZE < size())
    using drop_prefix = range<LOWER_BOUND + static_cast<std::int64_t>(PREFIX_SIZE),
                              UPPER_BOUND,
                              DELTA_VALUE,
                              iteration_direction>;

    template<std::uint64_t SUFFIX_SIZE> requires (SUFFIX_SIZE < size())
    using suffix = drop_prefix<size() - SUFFIX_SIZE>;

    template<std::uint64_t PREFIX_SIZE> requires (PREFIX_SIZE < size())
    using prefix = drop_suffix<size() - PREFIX_SIZE>;

    template<std::uint64_t STEP_BY_VALUE> requires (STEP_BY_VALUE > 0)
    using step_by = range<LOWER_BOUND, UPPER_BOUND, STEP_BY_VALUE, iteration_direction>;

    using reversed = range<LOWER_BOUND, UPPER_BOUND, DELTA_VALUE,
                           iteration_direction == detail::iteration_direction::forward
                           ? detail::iteration_direction::reverse
                           : detail::iteration_direction::forward>;

    //! MARK: Operators

    template<std::size_t INDEX>
    requires (INDEX < size())
    constexpr static auto get() {
        return LOWER_BOUND + INDEX;
    }

    constexpr auto operator==(const range& other_range) const -> bool = default;
    constexpr auto operator!=(const range& other_range) const -> bool = default;

    //! MARK: Iterator

    struct iterator {
        using value_type = std::int64_t;
        using pointer = value_type*;
        using reference = value_type&;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        constexpr explicit iterator(const value_type value,
                                    const value_type limit): m_value(value),
                                                             m_limit(limit) {}

        constexpr auto operator*() const { return m_value; }
        constexpr auto operator!=(const iterator& other) const { return m_value != other.m_value; }

        constexpr auto& operator++() {
            if constexpr (iteration_direction == detail::iteration_direction::forward) {
                m_value = m_value + static_cast<value_type>(DELTA_VALUE);
                m_value = std::min(m_value, m_limit);
            } else {
                m_value = m_value - static_cast<value_type>(DELTA_VALUE);
                m_value = std::max(m_value, m_limit);
            }

            return *this;
        }

    private:
        value_type m_value;
        value_type m_limit;
    };

    //! MARK: Iterators

    constexpr auto begin() const {
        if constexpr (iteration_direction == detail::iteration_direction::forward) {
            return iterator(LOWER_BOUND, UPPER_BOUND);
        } else {
            return iterator(UPPER_BOUND - 1, LOWER_BOUND - 1);
        }
    }

    constexpr auto end() const {
        if constexpr (iteration_direction == detail::iteration_direction::forward) {
            return iterator(UPPER_BOUND, UPPER_BOUND);
        } else {
            return iterator(LOWER_BOUND - 1, LOWER_BOUND - 1);
        }
    }
};

}

namespace apl {

//! MARK: Public Aliases

//! @brief A half-open range over the integers [LOWER_BOUND, UPPER_BOUND).

template<std::int64_t LOWER_BOUND, std::int64_t UPPER_BOUND> requires (UPPER_BOUND > LOWER_BOUND)
using range = impl::range<LOWER_BOUND, UPPER_BOUND, 1, detail::iteration_direction::forward>;

}
