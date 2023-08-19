//
// Created by David Spry on 29/1/2023.
//

#pragma once

#include "test_types.hpp"

#include <apl_threading/include/swsr_queue.hpp>
#include <gtest/gtest.h>

namespace apl::test {

TEST(swsr_queue, is_empty) {
    constexpr auto limit = 4u;
    auto swsr = swsr_queue<detail::copyable, limit>{};
    ASSERT_TRUE(swsr.is_empty());

    swsr.push({});
    ASSERT_FALSE(swsr.is_empty());

    swsr.pop();
    ASSERT_TRUE(swsr.is_empty());
}

TEST(swsr_queue, push_until_full) {
    constexpr auto limit = 128u;
    auto swsr = swsr_queue<detail::copyable, limit>{};

    for (auto i = 0; i < limit; ++i) {
        ASSERT_TRUE(
                swsr.push(
                        detail::copyable{1, 2, "3"}
                )
        );
    }

    ASSERT_FALSE(
            swsr.push(
                    detail::copyable{3, 4, "5"}
            )
    );
}

TEST(swsr_queue, pop_until_empty) {
    constexpr auto limit = 256u;
    auto swsr = swsr_queue<detail::copyable, limit>{};

    for (auto i = 0; i < limit; ++i) {
        swsr.push(
                detail::copyable{i, i + 1, "value3"}
        );
    }

    for (auto i = 0; i < limit; ++i) {
        const auto value = swsr.pop();
        ASSERT_TRUE(value.has_value());
        ASSERT_EQ(value.value().value1, i);
        ASSERT_EQ(value.value().value2, i + 1);
        ASSERT_EQ(value.value().value3, "value3");
    }

    ASSERT_FALSE(swsr.pop().has_value());
}

}
