//
// Created by David Spry on 22/1/2023.
//

#pragma once

#include <apl_utility/include/random.hpp>
#include <cstddef>
#include <gtest/gtest.h>
#include <ranges>

namespace apl::test {

TEST(random, initialise_from_same_seed) {
  for (auto i = 0u; i < 128u; ++i) {
    auto random1 = apl::random{i << 1u, i << 2u};
    auto random2 = apl::random{i << 1u, i << 2u};
    for (auto k = 0; k < 32; ++k) {
      ASSERT_EQ(
        std::make_tuple(
          random1(),
          random1.operator()<std::uint32_t>(),
          random1.operator()<bool>(),
          random1.uniform<double>(),
          std::lerp(1024.0, 2048.0, random1.uniform<double>())
        ),
        std::make_tuple(
          random2(),
          random2.operator()<std::uint32_t>(),
          random2.operator()<bool>(),
          random2.uniform<double>(),
          std::lerp(1024.0, 2048.0, random2.uniform<double>())
        )
      );
    }
  }
}

TEST(random, initialise_from_different_seeds) {
  for (auto i = 0u; i < 128u; ++i) {
    auto random1 = apl::random{(i + 0) << 1u, (i + 0) << 2u};
    auto random2 = apl::random{(i + 1) << 2u, (i + 1) << 4u};
    for (auto k = 0; k < 32; ++k) {
      ASSERT_NE(
        std::make_tuple(
          random1(),
          random1.operator()<std::uint32_t>(),
          random1.operator()<bool>(),
          random1.uniform<double>(),
          std::lerp(1024.0, 2048.0, random1.uniform<double>())
        ),
        std::make_tuple(
          random2(),
          random1.operator()<std::uint32_t>(),
          random1.operator()<bool>(),
          random1.uniform<double>(),
          std::lerp(1024.0, 2048.0, random1.uniform<double>())
        )
      );
    }
  }
}

TEST(random, uniform_range) {
  for (auto i = 0; i < 512; ++i) {
    auto random = apl::random(i << (i << 1u), i << (i << 4u));
    for (auto k = 0; k < 4096; ++k) {
      const auto value1 = random.uniform<float>();
      ASSERT_LE(value1, 1.0f);
      ASSERT_GE(value1, 0.0f);
    }
  }
}

}
