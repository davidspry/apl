//
// Created by David Spry on 15/10/2023.
//

#pragma once

#include <algorithm>
#include <apl_functional/include/unary.hpp>
#include <gtest/gtest.h>
#include <ranges>

namespace apl::test {

TEST(unary, abs) {
  ASSERT_TRUE(
    std::ranges::equal(
      std::views::iota(-10, 10) | std::views::transform(apl::abs),
      std::views::iota(-10, 10) | std::views::transform([](const auto x) { return std::abs(x); })
    )
  );
}

TEST(unary, add) {
  ASSERT_TRUE(
    std::ranges::equal(
      std::views::iota(1, 10) | std::views::transform(apl::add(5)),
      std::views::iota(6, 15)
    )
  );
}

TEST(unary, sub) {
  ASSERT_TRUE(
    std::ranges::equal(
      std::views::iota(1, 10) | std::views::transform(apl::sub(1)),
      std::views::iota(0, 9)
    )
  );
}

TEST(unary, sub_from) {
  ASSERT_TRUE(
    std::ranges::equal(
      std::views::iota(1, 10) | std::views::transform(apl::sub_from(10)),
      std::views::iota(1, 10) | std::views::transform([](const auto x) { return 10 - x; })
    )
  );
}

TEST(unary, mul) {
  ASSERT_TRUE(
    std::ranges::equal(
      std::views::iota(1, 10) | std::views::transform(apl::mul(2)),
      std::views::iota(1, 10) | std::views::transform([](const auto x) { return x << 1; })
    )
  );
}

TEST(unary, div) {
  ASSERT_TRUE(
    std::ranges::equal(
      std::views::iota(1, 10) | std::views::transform(apl::div(2)),
      std::views::iota(1, 10) | std::views::transform([](const auto x) { return x >> 1; })
    )
  );
}

TEST(unary, dividing) {
  ASSERT_TRUE(
    std::ranges::equal(
      std::views::iota(1, 10) | std::views::transform(apl::dividing(100)),
      std::views::iota(1, 10) | std::views::transform([](const auto x) { return 100 / x; })
    )
  );
}

TEST(unary, lsh) {
  ASSERT_TRUE(
    std::ranges::equal(
      std::views::iota(1, 5) | std::views::transform(apl::lsh(1)),
      std::views::iota(1, 5) | std::views::transform([](const auto x) { return x << 1; })
    )
  );
}

TEST(unary, rsh) {
  ASSERT_TRUE(
    std::ranges::equal(
      std::views::iota(1, 10) | std::views::transform(apl::rsh(1)),
      std::views::iota(1, 10) | std::views::transform([](const auto x) { return x >> 1; })
    )
  );
}

TEST(unary, identity) {
  ASSERT_TRUE(
    std::ranges::equal(
      std::views::iota(1, 10) | std::views::transform(apl::identity),
      std::views::iota(1, 10)
    )
  );
}

}
