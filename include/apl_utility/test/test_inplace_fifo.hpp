//! apl_utility_test
//! David Spry
//! Created on 9/1/2024

#pragma once

#include <gtest/gtest.h>
#include <ranges>

#include <apl_utility/include/inplace_fifo.hpp>

namespace apl::test {

TEST(inplace_fifo, initialisation) {
  constexpr auto size = 128;
  constexpr auto fifo = apl::inplace_fifo<float, size>{};
  ASSERT_TRUE(fifo.empty());
  ASSERT_EQ(fifo.size(), 0);
  ASSERT_EQ(fifo.capacity(), size);
  ASSERT_FALSE(fifo.full());
}

TEST(inplace_fifo, push_pop) {
  constexpr auto size = 8;
  auto fifo = apl::inplace_fifo<int, size>{};

  for (auto i = 0; i < size; ++i) {
    fifo.push_back(i);
    ASSERT_EQ(fifo.front(), 0);
    ASSERT_EQ(fifo.size(), i + 1);
  }

  ASSERT_TRUE(fifo.full());
  for (auto i = 0; i < size; ++i) {
    fifo.push_back(i);
    ASSERT_EQ(fifo.front(), (i + 1) % size);
    ASSERT_EQ(fifo.size(), size);
  }

  ASSERT_TRUE(fifo.full());
  fifo.push_back(std::ranges::to<std::vector>(std::views::iota(0, size << 1)));

  ASSERT_TRUE(fifo.full());
  for (auto i = 0; i < size; ++i) {
    ASSERT_EQ(fifo.front(), size + i);
    ASSERT_EQ(fifo.size(), size - i);
    fifo.pop_front();
  }

  ASSERT_TRUE(fifo.empty());
  fifo.push_back(std::ranges::to<std::vector>(std::views::iota(0, size >> 1)));

  ASSERT_EQ(fifo.size(), size >> 1);
  for (auto i = 0; i < size >> 1; ++i) {
    ASSERT_EQ(fifo.front(), i);
    ASSERT_EQ(fifo.size(), (size >> 1) - i);
    fifo.pop_front();
  }
}

}
