//
// Created by David Spry on 22/1/2023.
//

#pragma once

#include <apl_threading/include/seqlock.hpp>
#include <gtest/gtest.h>
#include <string_view>

namespace apl::test {

TEST(seqlock, initialise_default) {
  auto locked = apl::seqlock<std::string_view>{};
  auto loaded_value = std::string_view{};
  ASSERT_TRUE(locked.try_load(loaded_value));
  ASSERT_EQ(locked.load(loaded_value), std::string_view{});
}

TEST(seqlock, initialise_with_value) {
  auto locked = apl::seqlock<std::string_view>{"string"};
  auto loaded_value = std::string_view{};
  ASSERT_TRUE(locked.try_load(loaded_value));
  ASSERT_EQ(locked.load(loaded_value), std::string_view{"string"});
}

TEST(seqlock, store) {
  auto locked = apl::seqlock<std::string_view>{};
  auto lvalue = std::string_view("lvalue");
  auto loaded_value = std::string_view{};

  locked.store(lvalue);
  ASSERT_EQ(locked.load(loaded_value), lvalue);

  locked.store("rvalue");
  ASSERT_EQ(locked.load(loaded_value), "rvalue");
}

TEST(seqlock, store_and_load) {
  auto locked = apl::seqlock<std::size_t>{};
  auto loaded_value = std::size_t{};
  ASSERT_TRUE(locked.try_load(loaded_value));
  ASSERT_EQ(loaded_value, 0);
  ASSERT_EQ(locked.load(loaded_value), 0);

  const auto limit = 32uz;
  for (auto i = 0; i < limit; ++i) {
    locked.store(limit + i);
    ASSERT_TRUE(locked.try_load(loaded_value));
    ASSERT_EQ(loaded_value, limit + i);
    ASSERT_EQ(locked.load(loaded_value), limit + i);
  }
}

}
