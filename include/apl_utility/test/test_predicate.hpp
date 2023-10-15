//
// Created by David Spry on 15/10/2023.
//

#pragma once

#include <apl_utility/include/predicate.hpp>
#include <array>
#include <cstddef>
#include <gtest/gtest.h>
#include <ranges>

namespace apl::test {

TEST(predicate, filter_even) {
  static constexpr auto inputs = std::views::iota(1, 10);
  static constexpr auto result = std::array{2, 4, 6, 8};
  ASSERT_TRUE(
    std::ranges::equal(
      std::views::all(inputs) | std::views::filter(apl::even),
      std::views::all(result)
    )
  );
}

TEST(predicate, filter_eq) {
  static constexpr auto inputs = std::views::iota(1, 10);
  static constexpr auto result = std::views::iota(5, 6);
  ASSERT_TRUE(
    std::ranges::equal(
      std::views::all(inputs) | std::views::filter(apl::eq(5)),
      std::views::all(result)
    )
  );
}

TEST(predicate, filter_lt) {
  static constexpr auto inputs = std::views::iota(1, 10);
  static constexpr auto result = std::views::iota(1, 5);
  ASSERT_TRUE(
    std::ranges::equal(
      std::views::all(inputs) | std::views::filter(apl::lt(5)),
      std::views::all(result)
    )
  );
}

TEST(predicate, filter_leq) {
  static constexpr auto inputs = std::views::iota(1, 10);
  static constexpr auto result = std::views::iota(1, 6);
  ASSERT_TRUE(
    std::ranges::equal(
      std::views::all(inputs) | std::views::filter(apl::leq(5)),
      std::views::all(result)
    )
  );
}

TEST(predicate, filter_gt) {
  static constexpr auto inputs = std::views::iota(1, 10);
  static constexpr auto result = std::views::iota(6, 10);
  ASSERT_TRUE(
    std::ranges::equal(
      std::views::all(inputs) | std::views::filter(apl::gt(5)),
      std::views::all(result)
    )
  );
}

TEST(predicate, filter_geq) {
  static constexpr auto inputs = std::views::iota(1, 10);
  static constexpr auto result = std::views::iota(5, 10);
  ASSERT_TRUE(
    std::ranges::equal(
      std::views::all(inputs) | std::views::filter(apl::geq(5)),
      std::views::all(result)
    )
  );
}

TEST(predicate, comparable_types) {
  ASSERT_TRUE(
    std::ranges::equal(
      std::views::iota(10, 15 + 1),
      std::views::iota(10, 20) | std::views::filter(apl::leq(15.0))
    )
  );
}

}
