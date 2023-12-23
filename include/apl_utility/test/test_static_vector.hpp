#pragma once

#include <apl_utility/include/static_vector.hpp>
#include <gtest/gtest.h>
#include <ranges>
#include <span>
#include <string_view>

namespace apl::test {

TEST(static_vector, default_initialisation) {
  constexpr auto size = 64;
  const auto vector = apl::static_vector<float, size>{};
  ASSERT_TRUE(vector.empty());
  ASSERT_EQ(vector.begin(), vector.end());
  ASSERT_EQ(vector.capacity(), size);
}

TEST(static_vector, initialise_from_array) {
  static constexpr auto array = std::array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  static constexpr auto count = array.size();
  const auto vector = apl::static_vector<int, count>{
    std::span<const int, count>(array.begin(), count)
  };

  ASSERT_TRUE(not vector.empty());
  ASSERT_TRUE(std::ranges::equal(vector, std::views::iota(0, 10)));
}

TEST(static_vector, initialise_from_vector) {
  constexpr auto count = 16;
  const auto vector = apl::static_vector<int, count>{
    std::ranges::to<std::vector>(std::views::iota(0, count))
  };

  ASSERT_TRUE(not vector.empty());
  ASSERT_TRUE(std::ranges::equal(vector, std::views::iota(0, count)));
}

TEST(static_vector, bad_alloc) {
  constexpr auto capacity = 7;
  auto vector = apl::static_vector<std::string_view, capacity>{};

  for (auto i = 0; i < capacity; ++i) {
    ASSERT_NO_THROW(vector.push_back("le"));
  }

  ASSERT_EQ(vector.size(), capacity);
  ASSERT_THROW(vector.push_back("gt"), std::bad_alloc);

  ASSERT_NO_THROW(vector.erase(vector.begin()));
  ASSERT_NO_THROW(vector.push_back("le"));

  ASSERT_EQ(vector.size(), capacity);
  ASSERT_THROW(vector.push_back("gt"), std::bad_alloc);
}

TEST(static_vector, resize) {
  constexpr auto capacity = 8;
  auto vector = apl::static_vector<int, capacity>{};

  for (auto i = capacity; i >= 0; --i) {
    ASSERT_NO_THROW(vector.resize(i));
    ASSERT_EQ(vector.capacity(), capacity);
  }

  ASSERT_THROW(vector.resize(capacity + 1), std::bad_alloc);
  ASSERT_EQ(vector.capacity(), capacity);
}

}
