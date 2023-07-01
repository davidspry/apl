//
// Created by David Spry on 29/1/2023.
//

#pragma once

#include "test_types.hpp"

#include <apl/mwmr_queue.hpp>
#include <gtest/gtest.h>

namespace apl::test {

TEST(mwmr_queue, push_until_full) {
    constexpr auto limit = 128u;
    auto mwmr = mwmr_queue<detail::copyable, limit>{};

    for (auto i = 0; i < limit; ++i) {
        ASSERT_TRUE(
                mwmr.push(
                        detail::copyable{1, 2, "3"}
                )
        );
    }

    ASSERT_FALSE(
            mwmr.push(
                    detail::copyable{3, 4, "5"}
            )
    );
}

TEST(mwmr_queue, pop_until_empty) {
    constexpr auto limit = 256u;
    auto mwmr = mwmr_queue<detail::copyable, limit>{};

    for (auto i = 0; i < limit; ++i) {
        mwmr.push(
                detail::copyable{i, i + 1, "value3"}
        );
    }

    for (auto i = 0; i < limit; ++i) {
        auto value = mwmr.pop();

        ASSERT_TRUE(
                value.has_value()
        );

        ASSERT_EQ(value.value().value1, i);
        ASSERT_EQ(value.value().value2, i + 1);
        ASSERT_EQ(value.value().value3, "value3");
    }

    ASSERT_FALSE(
            mwmr.pop().has_value()
    );
}

}
