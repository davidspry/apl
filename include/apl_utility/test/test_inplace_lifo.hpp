#pragma once

#include <gtest/gtest.h>

#include <apl_utility/include/inplace_lifo.hpp>

namespace apl::test {

TEST(inplace_lifo, initialisation) {
  const auto lifo = apl::inplace_lifo<int, 4>{};
  ASSERT_EQ(lifo.empty(), true);
}

TEST(inplace_lifo, fill_then_empty) {
  constexpr auto size = 4;
  auto lifo = apl::inplace_lifo<int, size>{};

  for (auto i = 0; i < size; ++i) {
    ASSERT_FALSE(lifo.full());
    lifo.push(i);
  }

  ASSERT_TRUE(lifo.full());

  for (auto i = size - 1; i >= 0; --i) {
    ASSERT_FALSE(lifo.empty());
    ASSERT_EQ(i, lifo.top());
    lifo.pop();
  }

  ASSERT_TRUE(lifo.empty());
}

TEST(static_stack, alternately_push_and_pull) {
  constexpr auto size = 16;
  auto lifo = apl::inplace_lifo<std::size_t, size>{};

  for (auto i = 0; i < 16 * 2; ++i) {
    static auto should_push = true;

    if (should_push) {
      lifo.push(static_cast<std::size_t>(i));
    } else {
      auto result = lifo.top();
      ASSERT_EQ(result, i - 1);
    }

    should_push = not(should_push);
  }
}

}
