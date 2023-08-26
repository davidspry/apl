//
// Created by David Spry on 22/1/2023.
//

#pragma once

#include <apl_utility/include/mod.hpp>
#include <gtest/gtest.h>
#include <numeric>

namespace apl::test {

TEST(mod, initialise_default) {
  const auto m = mod<6>{};
  ASSERT_EQ(m.value(), 0);
}

TEST(mod, initialise_with_value) {
  constexpr auto limit = 8;
  const auto value = std::size_t{limit - 1};
  const auto m = mod32<limit>{value};
  ASSERT_EQ(m.value(), value);
}

TEST(mod, iterate_over_range) {
  constexpr auto m = apl::mod<4>();
  constexpr auto a = std::array{0ul, 1ul, 2ul, 3ul};
  auto index = 0;

  for (const auto i: m) {
    ASSERT_EQ(
      i,
      a[index++]
    );
  }

  ASSERT_EQ(
    std::accumulate(m.begin(), m.end(), 0ll),
    std::accumulate(a.begin(), a.end(), 0ul)
  );
}

TEST(mod, assignment) {
  constexpr auto limit = 12;
  auto m = apl::mod<limit>{};

  for (auto i = 1; i < limit * 2; ++i) {
    ASSERT_NE(m.value(), i % limit);
    m = i;
    ASSERT_EQ(m.value(), i % limit);
  }
}

TEST(mod, postfix_incr_decr_operators) {
  constexpr auto limit = 6;
  auto m = apl::mod<limit>{};

  for (auto i = 0; i < limit * 2; ++i) {
    ASSERT_EQ(m++.value(), i % limit);
  }

  for (auto i = limit * 2; i > 0; --i) {
    ASSERT_EQ(m--.value(), i % limit);
  }
}

TEST(mod, prefix_incr_decr_operators) {
  constexpr auto limit = 6;
  auto m = apl::mod<limit>{};

  for (auto i = 0; i < limit * 2; ++i) {
    ASSERT_EQ((++m).value(), (i + 1) % limit);
  }

  for (auto i = 0; i < limit * 2; ++i) {
    ASSERT_EQ(m.incremented().value(), (i + 1) % limit);
  }

  for (auto i = limit * 2; i > 0; --i) {
    ASSERT_EQ((--m).value(), (i + limit - 1) % limit);
  }

  for (auto i = limit * 2; i > 0; --i) {
    ASSERT_EQ(m.decremented().value(), (i + limit - 1) % limit);
  }
}

TEST(mod, addition_operators) {
  constexpr auto limit = 128uz;
  auto m = apl::mod<limit>{};

  for (auto i = 0; i < limit; ++i) {
    ASSERT_EQ(m.value(), i % limit);
    m = m + 1u;
  }

  for (auto i = 0; i < limit; ++i) {
    ASSERT_EQ(m.value(), i % limit);
    m += 1u;
  }

  for (auto i = 0; i < limit; ++i) {
    ASSERT_EQ(m.value(), i % limit);
    m = m.incremented_by(limit + 1);
  }

  for (auto i = 0; i < limit; i += 6) {
    ASSERT_EQ(m.value(), i % limit);
    m = m.incremented_by(6);
  }
}

TEST(mod, subtraction_operators) {
  constexpr auto limit = 128uz;
  auto m = apl::mod<limit>{};

  for (auto i = limit; i > 0; --i) {
    ASSERT_EQ(m.value(), i % limit);
    m = m - 1u;
  }

  for (auto i = limit; i > 0; --i) {
    ASSERT_EQ(m.value(), i % limit);
    m -= 1u;
  }

  for (auto i = limit; i > 0; --i) {
    ASSERT_EQ(m.value(), i % limit);
    m = m.decremented_by(limit + 1);
  }

  for (auto i = limit; i > 0; i -= 8) {
    ASSERT_EQ(m.value(), i % limit);
    m = m.decremented_by(8);
  }
}

TEST(mod, multiplication_operators) {
  constexpr auto limit = 65uz;
  auto m = apl::mod<limit>{};

  for (auto i = 0; i < limit; ++i) {
    m = i + 1;
    m = m * 3u;
    ASSERT_EQ(
      m.value(),
      (((i + 1) % limit) * 3) % limit
    );
  }

  for (auto i = 0; i < limit; ++i) {
    m = i + 1;
    m *= 3u;
    ASSERT_EQ(
      m.value(),
      (((i + 1) % limit) * 3) % limit
    );
  }
}

TEST(mod, division_operators) {
  constexpr auto limit = 15uz;
  auto m = apl::mod<limit>{};

  for (auto i = 0; i < limit; ++i) {
    m = i * i;
    m = m / 2u;
    ASSERT_EQ(
      m.value(),
      (((i * i) % limit) / 2) % limit
    );
  }

  for (auto i = 0; i < limit; ++i) {
    m = i * i;
    m /= 2u;
    ASSERT_EQ(
      m.value(),
      (((i * i) % limit) / 2) % limit
    );
  }
}

TEST(mod, left_shift_operators) {
  constexpr auto limit = 5uz;
  auto m = apl::mod<limit>{};

  for (auto i = 0; i < limit; ++i) {
    m = i;
    m = m << 1u;
    ASSERT_EQ(
      m.value(),
      (i << 1) % limit
    );
  }

  for (auto i = 0; i < limit; ++i) {
    m = i;
    m <<= 1u;
    ASSERT_EQ(
      m.value(),
      (i << 1) % limit
    );
  }
}

TEST(mod, right_shift_operators) {
  constexpr auto limit = 5uz;
  auto m = apl::mod<limit>{};

  for (auto i = 0; i < limit; ++i) {
    m = i + 640;
    m = m >> 1u;
    ASSERT_EQ(
      m.value(),
      ((i + 640) >> 1) % limit
    );
  }

  for (auto i = 0; i < limit; ++i) {
    m = i + 640;
    m >>= 1u;
    ASSERT_EQ(
      m.value(),
      ((i + 640) >> 1) % limit
    );
  }
}

TEST(mod, bitwise_and_operators) {
  constexpr auto limit = 5uz;
  auto m = apl::mod<limit>{};

  for (auto i = 0; i < limit; ++i) {
    m = i * 8;
    m = m & 9u;
    ASSERT_EQ(
      m.value(),
      (((i * 8) % limit) & 9u) % limit
    );
  }

  for (auto i = 0; i < limit; ++i) {
    m = i * 8;
    m &= 9u;
    ASSERT_EQ(
      m.value(),
      (((i * 8) % limit) & 9u) % limit
    );
  }
}

TEST(mod, bitwise_or_operators) {
  constexpr auto limit = 5uz;
  auto m = apl::mod<limit>{};

  for (auto i = 0; i < limit; ++i) {
    m = i;
    m = m | 0xffu;
    ASSERT_EQ(
      m.value(),
      ((i % limit) | 0xffu) % limit
    );
  }

  for (auto i = 0; i < limit; ++i) {
    m = i;
    m |= 0xffu;
    ASSERT_EQ(
      m.value(),
      ((i % limit) | 0xffu) % limit
    );
  }
}

TEST(mod, bitwise_xor_operators) {
  constexpr auto limit = 5uz;
  auto m = apl::mod<limit>{};

  for (auto i = 0; i < limit; ++i) {
    m = i * 4;
    m = m ^ 0xcafeu;
    ASSERT_EQ(
      m.value(),
      (((i * 4) % limit) ^ 0xcafeu) % limit
    );
  }

  for (auto i = 0; i < limit; ++i) {
    m = i * 4;
    m ^= 0xcafeu;
    ASSERT_EQ(
      m.value(),
      (((i * 4) % limit) ^ 0xcafeu) % limit
    );
  }
}

}
