//! apl
//! David Spry
//! Created on 26/8/2023

#pragma once

#include <apl_utility/include/unpack.hpp>
#include <gtest/gtest.h>

namespace apl::test {

TEST(unpack, enum_values) {
  enum class values {
    value1,
    value2,
    value3
  };

  static auto index = 0uz;
  static constexpr auto expected = std::array{
    values::value1,
    values::value2,
    values::value3,
    values::value2,
    values::value1,
  };

  apl::unpack<values::value1,
              values::value2,
              values::value3,
              values::value2,
              values::value1>(
    []<values VALUE>() {
      ASSERT_TRUE(VALUE == expected[index++]);
    }
  );
}

TEST(unpack, types) {
  static constexpr auto values = std::make_tuple(0, 1u, 2.0f, 3.0, 4uz);
  static auto index = 0;
  std::apply([]<class... T>(T&& ...) {
    apl::unpack<T...>(
      []<class U>() {
        ASSERT_EQ(
          std::get<std::decay_t<U>>(values),
          static_cast<U>(index++)
        );
      }
    );
  }, values);
}

TEST(enumerate, nttp) {
  apl::enumerate<0, 1, 2, 3, 4>([]<std::size_t INDEX, std::size_t VALUE> {
    ASSERT_EQ(INDEX, VALUE);
  });
}

TEST(enumerate, types) {
  static constexpr auto values = std::make_tuple(0, 1u, 2.0f, 3.0, 4uz);
  std::apply([]<class... T>(T&& ...) {
    apl::enumerate<T...>(
      []<std::size_t INDEX, class U> {
        ASSERT_TRUE(
          (std::same_as<U, decltype(std::get<INDEX>(values))>)
        );
      });
  }, values);
}

}
