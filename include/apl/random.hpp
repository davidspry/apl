//
// Created by David Spry on 24/2/2023.
//

#pragma once

#include <concepts>
#include <cstdint>
#include <limits>

namespace apl {

//! A fast random-number generator with constant time complexity, based on the 'xorshift' algorithm.

class random final {
    using value_type = std::uint64_t;

    mutable value_type m_state{};

public:
    constexpr random():
            m_state(1) {}

    constexpr explicit random(const value_type seed):
            m_state(1 + seed) {
        operator()();
    }

    constexpr auto operator()() const -> value_type {
        m_state ^= m_state << 13;
        m_state ^= m_state >> 7;
        m_state ^= m_state << 17;
        return m_state;
    }

    constexpr auto seed(const value_type seed) -> random& {
        m_state = seed;
        return *this;
    }

    //! Return true in approximately 50% of cases.
    constexpr auto chance() const -> bool {
        return 0.5f < real<float>();
    }

    //! Produce a floating-point number in the range [0, 1]
    template<std::floating_point real_type>
    constexpr auto real() const -> real_type {
        return static_cast<real_type>(operator()()) /
               static_cast<real_type>(std::numeric_limits<value_type>::max());
    }

    //! Produce a floating-point number in the range [minimum, maximum]
    template<std::floating_point real_type>
    constexpr auto real(const real_type minimum, const real_type maximum) const -> real_type {
        return minimum + real<real_type>() * (maximum - minimum);
    }

    //! Produce an integer in the range [minimum, maximum]
    template<std::integral integer_type>
    constexpr auto integer(const integer_type minimum, const integer_type maximum) const -> integer_type {
        return static_cast<integer_type>(real<float>(static_cast<float>(minimum),
                                                     static_cast<float>(maximum)));
    }
};

}
