//
// Created by David Spry on 10/5/2023.
//

#pragma once

#include "test_types.hpp"

#include <apl_threading/include/double_buffer.hpp>
#include <gtest/gtest.h>

namespace apl::test {

TEST(double_buffer, initialise_default) {
  const auto double_buffer = apl::double_buffer<detail::pod_type>{};
  const auto value = double_buffer.get();
  ASSERT_EQ(value, detail::pod_type{});
}

TEST(double_buffer, initialise_from_initial_value) {
  const auto initial_value = detail::pod_type{1, 2, "3"};
  auto double_buffer = apl::double_buffer<detail::pod_type>{initial_value};
  const auto value = double_buffer.get();
  ASSERT_EQ(value, initial_value);
}

TEST(double_buffer, set_value) {
  auto double_buffer = apl::double_buffer<detail::pod_type>{};
  const auto new_value = detail::pod_type{1, 2, "3"};
  double_buffer.set(new_value);
  const auto value = double_buffer.get();
  ASSERT_EQ(value, new_value);
}

TEST(double_buffer, update_value) {
  auto double_buffer = apl::double_buffer<detail::pod_type>{};

  {
    auto modifier = double_buffer.write_ptr();
    modifier->value1 = 1;
    modifier->value2 = 2;
    modifier->value3 = "3";
  }

  const auto value = double_buffer.get();
  ASSERT_EQ(value.value1, 1);
  ASSERT_EQ(value.value2, 2);
  ASSERT_EQ(value.value3, "3");
}

}
