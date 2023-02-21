//
// Created by David Spry on 22/1/2023.
//

#pragma once

#include "test_types.hpp"

#include <apl/concurrency/seqlock.hpp>
#include <gtest/gtest.h>

namespace apl::test {

TEST(seqlock, initialise_default) {
    auto locked = apl::seqlock<std::string>{};
    ASSERT_TRUE(locked.try_load().has_value());
    ASSERT_EQ(locked.load(), std::string{});
}

TEST(seqlock, initialise_with_value) {
    auto locked = apl::seqlock<std::string>{"string"};
    ASSERT_TRUE(locked.try_load().has_value());
    ASSERT_EQ(locked.load(), std::string{"string"});
}

TEST(seqlock, store) {
    auto locked = apl::seqlock<std::string_view>{};
    auto lvalue = std::string_view("lvalue");

    locked.store(lvalue);
    ASSERT_EQ(locked.load(), lvalue);

    locked.store("rvalue");
    ASSERT_EQ(locked.load(), "rvalue");
}

TEST(seqlock, store_and_load) {
    auto locked = apl::seqlock<std::size_t>{};
    ASSERT_TRUE(locked.try_load().has_value());
    ASSERT_EQ(locked.try_load().value(), 0);
    ASSERT_EQ(locked.load(), 0);

    const auto limit = 32uz;
    for (auto i = 0; i < limit; ++i) {
        locked.store(limit + i);
        ASSERT_TRUE(locked.try_load().has_value());
        ASSERT_EQ(locked.try_load().value(), limit + i);
        ASSERT_EQ(locked.load(), limit + i);
    }
}

}
