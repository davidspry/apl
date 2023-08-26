//! apl
//! David Spry
//! Created on 30/3/2023

#pragma once

#include <apl_utility/include/repeat.hpp>
#include <concepts>
#include <cstddef>
#include <gtest/gtest.h>

namespace apl::test {

TEST(repeat, range) {
  constexpr auto tuple = std::make_tuple(1, 2, 3, 4);
  apl::repeat<2, 4>([&tuple]<std::int64_t INDEX>() {
    ASSERT_TRUE(2 <= INDEX && INDEX < 4);
    ASSERT_TRUE(INDEX == std::get<INDEX>(tuple) - 1);
  });
}

TEST(repeat, count) {
  apl::repeat<8>([]<std::int64_t INDEX>() {
    ASSERT_TRUE(0 <= INDEX && INDEX < 8);
  });
}

}
