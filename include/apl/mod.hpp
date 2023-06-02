//
// Created by David Spry on 22/1/2023.
//

#pragma once

#include <apl/range.hpp>
#include <concepts>
#include <limits>
#include <iterator>

namespace apl {

//! @brief An unsigned-integer type that uses the modulo operator to ensure that the value stays within [0, LIMIT).

template<std::size_t LIMIT, std::unsigned_integral unsigned_integral_type = std::size_t> requires (LIMIT > 0)
class mod {
    static_assert(
            LIMIT <= std::numeric_limits<unsigned_integral_type>::max(),
            "the given limit value cannot be represented by the underlying value-type"
    );

    unsigned_integral_type m_value;

    static constexpr auto wrap(const unsigned_integral_type value) {
        if constexpr (std::has_single_bit(LIMIT)) {
            return value & (LIMIT - 1);
        } else {
            return value % LIMIT;
        }
    }

public:
    constexpr mod(const mod&) = default;
    constexpr mod(mod&&) noexcept = default;
    constexpr mod& operator=(const mod&) = default;
    constexpr mod& operator=(mod&&) noexcept = default;

    constexpr explicit mod() = default;
    constexpr explicit mod(const unsigned_integral_type value): m_value(wrap(value)) {}

    //! MARK: - Public Interface

    [[nodiscard]] constexpr unsigned_integral_type value() const { return m_value; }

    [[nodiscard]]
    constexpr auto incremented() -> mod {
        return this->operator++();
    }

    [[nodiscard]]
    constexpr auto incremented_by(const std::size_t delta) -> mod {
        return this->operator+(delta);
    }

    [[nodiscard]]
    constexpr auto decremented() -> mod {
        return this->operator--();
    }

    [[nodiscard]]
    constexpr auto decremented_by(const std::size_t delta) -> mod {
        return this->operator-(delta);
    }

    //! MARK: - Conversion Operators

    [[nodiscard]] constexpr explicit operator int() { return static_cast<int>(m_value); }
    [[nodiscard]] constexpr explicit operator long() { return static_cast<long>(m_value); }
    [[nodiscard]] constexpr explicit operator long long() { return static_cast<long long>(m_value); }
    [[nodiscard]] constexpr explicit operator unsigned() { return static_cast<unsigned>(m_value); }
    [[nodiscard]] constexpr explicit operator unsigned long() { return static_cast<unsigned long>(m_value); }
    [[nodiscard]] constexpr explicit operator unsigned long long() { return static_cast<unsigned long long>(m_value); }
    [[nodiscard]] constexpr explicit operator float() { return static_cast<float>(m_value); }
    [[nodiscard]] constexpr explicit operator double() { return static_cast<double>(m_value); }

    //! MARK: - Assignment

    constexpr auto operator=(const unsigned_integral_type value) -> mod& {
        m_value = wrap(value);
        return *this;
    }

    //! MARK: - Increment/Decrement Operators

    constexpr auto operator++() -> mod {
        m_value = wrap(m_value + 1);
        return mod(m_value);
    }

    constexpr auto operator++([[maybe_unused]] int) -> mod {
        const unsigned_integral_type value = m_value;
        m_value = wrap(m_value + 1);
        return mod(value);
    }

    constexpr auto operator--() -> mod {
        m_value = wrap(m_value + LIMIT - 1);
        return mod(m_value);
    }

    constexpr auto operator--([[maybe_unused]] int) -> mod {
        const unsigned_integral_type value = m_value;
        m_value = wrap(m_value + LIMIT - 1);
        return mod(value);
    }

    //! MARK: - Arithmetic Operators

    constexpr auto operator+(std::unsigned_integral auto other_value) -> mod {
        return mod(m_value + other_value);
    }

    constexpr auto operator+=(std::unsigned_integral auto other_value) -> mod& {
        m_value = this->operator+(other_value).m_value;
        return *this;
    }

    constexpr auto operator-(std::unsigned_integral auto other_value) -> mod {
        return mod(m_value - wrap(other_value) + LIMIT);
    }

    constexpr auto operator-=(std::unsigned_integral auto other_value) -> mod& {
        m_value = this->operator-(other_value).m_value;
        return *this;
    }

    constexpr auto operator*(std::unsigned_integral auto other_value) -> mod {
        return mod(m_value * other_value);
    }

    constexpr auto operator*=(std::unsigned_integral auto other_value) -> mod& {
        m_value = this->operator*(other_value).m_value;
        return *this;
    }

    constexpr auto operator/(std::unsigned_integral auto other_value) -> mod {
        return mod(m_value / other_value);
    }

    constexpr auto operator/=(std::unsigned_integral auto other_value) -> mod& {
        m_value /= other_value;
        return *this;
    }

    constexpr auto operator<<(std::unsigned_integral auto other_value) -> mod {
        return mod(m_value << other_value);
    }

    constexpr auto operator<<=(std::unsigned_integral auto other_value) -> mod& {
        m_value = this->operator<<(other_value).m_value;
        return *this;
    }

    constexpr auto operator>>(std::unsigned_integral auto other_value) -> mod {
        return mod(m_value >> other_value);
    }

    constexpr auto operator>>=(std::unsigned_integral auto other_value) -> mod& {
        m_value = this->operator>>(other_value).m_value;
        return *this;
    }

    constexpr auto operator&(std::unsigned_integral auto other_value) -> mod {
        return mod(m_value & other_value);
    }

    constexpr auto operator&=(std::unsigned_integral auto other_value) -> mod& {
        m_value = this->operator&(other_value).m_value;
        return *this;
    }

    constexpr auto operator|(std::unsigned_integral auto other_value) -> mod {
        return mod(m_value | other_value);
    }

    constexpr auto operator|=(std::unsigned_integral auto other_value) -> mod& {
        m_value = this->operator|(other_value).m_value;
        return *this;
    }

    constexpr auto operator^(std::unsigned_integral auto other_value) -> mod {
        return mod(m_value ^ other_value);
    }

    constexpr auto operator^=(std::unsigned_integral auto other_value) -> mod& {
        m_value = this->operator^(other_value).m_value;
        return *this;
    }

    //! MARK: - Iterators

    constexpr auto begin() const { return range<0, LIMIT>{}.begin(); }
    constexpr auto end() const { return range<0, LIMIT>{}.end(); }

    constexpr auto rbegin() const { return (typename range<0, LIMIT>::reversed()).begin(); }
    constexpr auto rend() const { return (typename range<0, LIMIT>::reversed()).end(); }
};

//! MARK: Aliases

template<std::size_t LIMIT>
using mod8 = mod<LIMIT, std::uint8_t>;

template<std::size_t LIMIT>
using mod16 = mod<LIMIT, std::uint16_t>;

template<std::size_t LIMIT>
using mod32 = mod<LIMIT, std::uint32_t>;

template<std::size_t LIMIT>
using mod64 = mod<LIMIT, std::uint64_t>;

}
