//
// Created by David Spry on 15/10/2023.
//

#pragma once

#include <algorithm>
#include <apl_functional/include/combine.hpp>
#include <apl_functional/include/unary.hpp>
#include <apl_functional/include/predicate.hpp>
#include <gtest/gtest.h>
#include <ranges>

namespace apl::test {

static constexpr auto sum = [](const auto&&... values) {
  return (values + ...);
};

TEST(combine, apply) {
  ASSERT_EQ(true, apl::apply(5)(apl::eq)(5));
  ASSERT_EQ(10, apl::apply(5)(apl::add)(5));
  ASSERT_EQ(10, apl::apply(5)(apl::apply(5)(apl::add)));
  ASSERT_EQ(20, apl::apply(10, 5, 5)(sum));
}

TEST(combine, partial_apply) {
  ASSERT_EQ(15, apl::partial_apply(10)(sum)(5));

  ASSERT_EQ(15, apl::apply(10)(apl::partial(sum))(5));
}

TEST(combine, compose) {
  ASSERT_EQ(50, apl::compose(apl::sub_from(100), apl::add(25))(25));
}

TEST(combine, compose_nested) {
  ASSERT_EQ(20, apl::compose(
    apl::mul(4),
    apl::compose(apl::sub_from(10), apl::add(2))
  )(3));

  ASSERT_EQ(4, apl::compose(
    apl::compose(apl::identity, apl::identity),
    apl::compose(apl::add(2), apl::add(2))
  )(0));
}

}
