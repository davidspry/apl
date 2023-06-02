//
// Created by David Spry on 30/3/2023.
//

#pragma once

#include "test_types.hpp"

#include <apl/utility.hpp>
#include <concepts>
#include <cstddef>
#include <gtest/gtest.h>

namespace apl::test {

TEST(utility, constexpr_for_range) {
    constexpr auto tuple = std::make_tuple(1, 2, 3, 4);
    apl::constexpr_for<2, 4>([&tuple]<std::int64_t INDEX>() {
        ASSERT_TRUE(2 <= INDEX && INDEX < 4);
        ASSERT_TRUE(INDEX == std::get<INDEX>(tuple) - 1);
    });
}

TEST(utility, constexpr_for_each_non_type_parameter) {
    enum class values { value1, value2, value3 };
    apl::constexpr_for_each<values::value1,
                            values::value2,
                            values::value3>
        ([]<values VALUE>() {
            ASSERT_TRUE(
                VALUE == values::value1 ||
                VALUE == values::value2 ||
                VALUE == values::value3
            );
        });
}

TEST(utility, constexpr_for_each_type_parameter) {
    apl::constexpr_for_each<int, float, bool>
        ([]<class T>() {
            ASSERT_TRUE(
                (std::same_as<T, int> ||
                 std::same_as<T, float> ||
                 std::same_as<T, bool>)
            );
        });
}

}
