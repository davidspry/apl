//! apl
//! David Spry
//! Created on 22/1/2023

#pragma once

#include <apl_utility/include/range.hpp>
#include <bit>
#include <concepts>
#include <limits>
#include <iterator>

namespace apl {

//! @brief An unsigned-integer type that uses the modulo operator to ensure that the value stays within [0, LIMIT).

template<
  std::size_t LIMIT,
  std::unsigned_integral unsigned_t = std::size_t
> requires (LIMIT > 0 && LIMIT <= std::numeric_limits<unsigned_t>::max())
class mod {
  unsigned_t m_value;

  static constexpr auto wrap(const unsigned_t value) -> unsigned_t {
    if constexpr (std::has_single_bit(LIMIT)) {
      return value & static_cast<unsigned_t>(LIMIT - 1);
    } else {
      return value % static_cast<unsigned_t>(LIMIT);
    }
  }

public:
  constexpr mod(const mod&) = default;
  constexpr mod(mod&&) noexcept = default;
  constexpr mod& operator=(const mod&) = default;
  constexpr mod& operator=(mod&&) noexcept = default;
  constexpr explicit mod() = default;
  constexpr explicit mod(const unsigned_t value): m_value(wrap(value)) {}

  // MARK: - Public Interface

  [[nodiscard]] constexpr unsigned_t value() const {
    return m_value;
  }

  [[nodiscard]]
  constexpr auto increment() -> mod& {
    return this->operator+=(1);
  }

  [[nodiscard]]
  constexpr auto increment_by(const unsigned_t delta) -> mod& {
    return this->operator+=(delta);
  }

  [[nodiscard]]
  constexpr auto increment_by(const mod delta) -> mod& {
    return this->operator+=(delta);
  }

  [[nodiscard]]
  constexpr auto decrement() -> mod& {
    return this->operator-=(1);
  }

  [[nodiscard]]
  constexpr auto decrement_by(const unsigned_t delta) -> mod& {
    return this->operator-=(delta);
  }

  [[nodiscard]]
  constexpr auto decrement_by(const mod delta) -> mod& {
    return this->operator-=(delta);
  }

  [[nodiscard]]
  constexpr auto incremented() const -> mod {
    return this->operator+(1);
  }

  [[nodiscard]]
  constexpr auto incremented_by(const unsigned_t delta) const -> mod {
    return this->operator+(delta);
  }

  [[nodiscard]]
  constexpr auto incremented_by(const mod delta) const -> mod {
    return this->operator+(delta);
  }

  [[nodiscard]]
  constexpr auto decremented() const -> mod {
    return this->operator-(1);
  }

  [[nodiscard]]
  constexpr auto decremented_by(const unsigned_t delta) -> mod {
    return this->operator-(delta);
  }

  [[nodiscard]]
  constexpr auto decremented_by(const mod delta) -> mod {
    return this->operator-(delta);
  }

  // MARK: - Assignment

  constexpr auto operator=(const unsigned_t value) -> mod& {
    m_value = wrap(value);
    return *this;
  }

  // MARK: - Equality

  constexpr auto operator<=>(const mod&) const -> std::strong_ordering = default;

  // MARK: - Increment/Decrement Operators

  constexpr auto operator++() -> mod {
    m_value = wrap(m_value + 1);
    return mod(m_value);
  }

  constexpr auto operator++([[maybe_unused]] int) -> mod {
    const unsigned_t value = m_value;
    m_value = wrap(m_value + 1);
    return mod(value);
  }

  constexpr auto operator--() -> mod {
    m_value = wrap(m_value + LIMIT - 1);
    return mod(m_value);
  }

  constexpr auto operator--([[maybe_unused]] int) -> mod {
    const unsigned_t value = m_value;
    m_value = wrap(m_value + LIMIT - 1);
    return mod(value);
  }

  // MARK: - Arithmetic Operators

  constexpr auto operator+(const unsigned_t other_value) const -> mod {
    return mod(m_value + other_value);
  }

  constexpr auto operator+(const mod other) const -> mod {
    return operator+(other.m_value);
  }

  constexpr auto operator+=(const unsigned_t other_value) -> mod& {
    m_value = this->operator+(other_value).m_value;
    return *this;
  }

  constexpr auto operator+=(const mod other) -> mod& {
    return operator+=(other.m_value);
  }

  constexpr auto operator-(const unsigned_t other_value) const -> mod {
    return mod(m_value - wrap(other_value) + LIMIT);
  }

  constexpr auto operator-(const mod other) const -> mod {
    return operator-(other.m_value);
  }

  constexpr auto operator-=(const unsigned_t other_value) -> mod& {
    m_value = this->operator-(other_value).m_value;
    return *this;
  }

  constexpr auto operator-=(const mod other) -> mod& {
    return operator-=(other.m_value);
  }

  constexpr auto operator*(const unsigned_t other_value) const -> mod {
    return mod(m_value * other_value);
  }

  constexpr auto operator*(const mod other) const -> mod {
    return operator*(other.m_value);
  }

  constexpr auto operator*=(const unsigned_t other_value) -> mod& {
    m_value = this->operator*(other_value).m_value;
    return *this;
  }

  constexpr auto operator*=(const mod other) -> mod& {
    return operator*=(other.m_value);
  }

  constexpr auto operator/(const unsigned_t other_value) const -> mod {
    return mod(m_value / other_value);
  }

  constexpr auto operator/(const mod other) const -> mod {
    return operator/(other.m_value);
  }

  constexpr auto operator/=(const unsigned_t other_value) -> mod& {
    m_value /= other_value;
    return *this;
  }

  constexpr auto operator/=(const mod other) -> mod& {
    return operator/=(other.m_value);
  }

  constexpr auto operator<<(const unsigned_t other_value) const -> mod {
    return mod(m_value << other_value);
  }

  constexpr auto operator<<(const mod other) const -> mod {
    return operator<<(other.m_value);
  }

  constexpr auto operator<<=(const unsigned_t other_value) -> mod& {
    m_value = this->operator<<(other_value).m_value;
    return *this;
  }

  constexpr auto operator<<=(const mod other) -> mod& {
    return operator<<=(other.m_value);
  }

  constexpr auto operator>>(const unsigned_t other_value) const -> mod {
    return mod(m_value >> other_value);
  }

  constexpr auto operator>>(const mod other) const -> mod {
    return operator>>(other.m_value);
  }

  constexpr auto operator>>=(const unsigned_t other_value) -> mod& {
    m_value = this->operator>>(other_value).m_value;
    return *this;
  }

  constexpr auto operator>>=(const mod other) -> mod& {
    return operator>>=(other.m_value);
  }

  constexpr auto operator&(const unsigned_t other_value) const -> mod {
    return mod(m_value & other_value);
  }

  constexpr auto operator&(const mod other) const -> mod {
    return operator&(other.m_value);
  }

  constexpr auto operator&=(const unsigned_t other_value) -> mod& {
    m_value = this->operator&(other_value).m_value;
    return *this;
  }

  constexpr auto operator&=(const mod other) -> mod& {
    return operator&=(other.m_value);
  }

  constexpr auto operator|(const unsigned_t other_value) const -> mod {
    return mod(m_value | other_value);
  }

  constexpr auto operator|(const mod other) const -> mod {
    return operator|(other.m_value);
  }

  constexpr auto operator|=(const unsigned_t other_value) -> mod& {
    m_value = this->operator|(other_value).m_value;
    return *this;
  }

  constexpr auto operator|=(const mod other) -> mod& {
    return operator|=(other.m_value);
  }

  constexpr auto operator^(const unsigned_t other_value) const -> mod {
    return mod(m_value ^ other_value);
  }

  constexpr auto operator^(const mod other) const -> mod {
    return operator^(other.m_value);
  }

  constexpr auto operator^=(const unsigned_t other_value) -> mod& {
    m_value = this->operator^(other_value).m_value;
    return *this;
  }

  constexpr auto operator^=(const mod other) -> mod& {
    return operator^=(other.m_value);
  }

  // MARK: - Iterators

  //! @formatter:off
  constexpr auto begin() const  { return range<0, LIMIT>{}.begin(); }
  constexpr auto end() const    { return range<0, LIMIT>{}.end(); }
  constexpr auto rbegin() const { return (typename range<0, LIMIT>::reversed()).begin(); }
  constexpr auto rend() const   { return (typename range<0, LIMIT>::reversed()).end(); }
  //! @formatter:on

  // MARK: - Conversion Operators

  //! @formatter:off
  [[nodiscard]] constexpr explicit operator int()                { return static_cast<int>(m_value); }
  [[nodiscard]] constexpr explicit operator long()               { return static_cast<long>(m_value); }
  [[nodiscard]] constexpr explicit operator long long()          { return static_cast<long long>(m_value); }
  [[nodiscard]] constexpr explicit operator unsigned()           { return static_cast<unsigned>(m_value); }
  [[nodiscard]] constexpr explicit operator unsigned long()      { return static_cast<unsigned long>(m_value); }
  [[nodiscard]] constexpr explicit operator unsigned long long() { return static_cast<unsigned long long>(m_value); }
  [[nodiscard]] constexpr explicit operator float()              { return static_cast<float>(m_value); }
  [[nodiscard]] constexpr explicit operator double()             { return static_cast<double>(m_value); }
  //! @formatter:on
};

// MARK: - Aliases

template<std::size_t LIMIT>
using mod8 = mod<LIMIT, std::uint8_t>;

template<std::size_t LIMIT>
using mod16 = mod<LIMIT, std::uint16_t>;

template<std::size_t LIMIT>
using mod32 = mod<LIMIT, std::uint32_t>;

template<std::size_t LIMIT>
using mod64 = mod<LIMIT, std::uint64_t>;

}
