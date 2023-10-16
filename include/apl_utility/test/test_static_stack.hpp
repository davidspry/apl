#pragma once

#include <apl_utility/include/static_stack.hpp>
#include <gtest/gtest.h>
#include <ranges>

namespace apl::test {

TEST(static_stack, initialisation) {
  auto lifo = apl::static_stack<int, 4>{};
  ASSERT_EQ(lifo.capacity(), 4);
  ASSERT_EQ(lifo.is_full(), false);
  ASSERT_EQ(lifo.pull().has_value(), false);
}

TEST(static_stack, fill_then_empty) {
  constexpr auto size = 4;
  auto lifo = apl::static_stack<int, size>{};

  for (auto i = 0; i < size; ++i) {
    ASSERT_FALSE(lifo.is_full());
    lifo.push(i);
  }

  ASSERT_TRUE(lifo.is_full());

  for (auto i = size - 1; i >= 0; --i) {
    auto result = lifo.pull();
    ASSERT_EQ(result.has_value(), true);
    ASSERT_EQ(result.value(), i);
  }

  ASSERT_FALSE(lifo.pull().has_value());
}

TEST(static_stack, alternately_push_and_pull) {
  constexpr auto size = 16;
  auto lifo = apl::static_stack<std::size_t, size>{};

  for (auto i = 0; i < 16 * 2; ++i) {
    static auto should_push = true;

    if (should_push) {
      lifo.push(static_cast<std::size_t>(i));
    } else {
      auto result = lifo.pull();
      ASSERT_EQ(result.has_value(), true);
      ASSERT_EQ(result.value(), i - 1);
    }

    should_push = not(should_push);
  }
}

TEST(static_stack, iterators) {
  constexpr auto size = 16;
  for (const auto limit: {size >> 0, size >> 1, size >> 2, size >> 3}) {
    auto lifo = apl::static_stack<std::size_t, size>{};
    for (auto i = 0uz; i < limit; ++i) {
      lifo.push(i);
    }

    ASSERT_TRUE(
      std::ranges::equal(
        std::views::reverse(std::views::iota(0, limit)),
        lifo
      )
    );
  }
}

}
