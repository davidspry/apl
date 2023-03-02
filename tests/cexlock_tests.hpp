//
// Created by David Spry on 22/1/2023.
//

#pragma once

#include "test_types.hpp"

#include <apl/concurrency/cexlock.hpp>
#include <cstddef>
#include <gtest/gtest.h>
#include <string_view>

namespace apl::test {

TEST(cexlock, initialise_default) {
    auto locked = apl::cexlock<std::string_view>{};
    auto reader = locked.lock_read();
    ASSERT_FALSE(reader.is_null());
    ASSERT_EQ(reader->size(), 0);
}

TEST(cexlock, initialise_with_value) {
    auto locked = apl::cexlock<std::string_view>{"string"};
    auto reader = locked.lock_read();
    ASSERT_FALSE(reader.is_null());
    ASSERT_STREQ(reader->data(), "string");
}

TEST(cexlock, lock_read) {
    constexpr auto value = 32uz;
    auto locked = apl::cexlock<std::size_t>{value};
    auto reader = locked.lock_read();

    ASSERT_EQ(*reader, value);
    ASSERT_TRUE(locked.lock_read().is_null());
    ASSERT_EQ(*reader, value);
}

TEST(cexlock, write) {
    constexpr auto first = 32uz;
    constexpr auto limit = 16uz;
    auto locked = apl::cexlock<std::size_t>{first};
    ASSERT_EQ(*locked.lock_read(), first);

    for (auto i = 0uz; i < limit; ++i) {
        locked.write(i + 1);
    }

    ASSERT_EQ(*locked.lock_read(), limit);
}

TEST(cexlock, copy_read_then_write) {
    constexpr auto first = detail::pod_type{1, 2, "3"};
    constexpr auto other = detail::pod_type{4, 5, "6"};
    auto locked = apl::cexlock<detail::pod_type>{first};

    ASSERT_EQ(locked.lock_read()->value1, first.value1);
    ASSERT_EQ(locked.lock_read()->value2, first.value2);
    ASSERT_EQ(locked.lock_read()->value3, first.value3);

    auto value = locked.copy_read();
    value.value1 = other.value1;
    value.value2 = other.value2;
    value.value3 = other.value3;

    ASSERT_EQ(locked.lock_read()->value1, first.value1);
    ASSERT_EQ(locked.lock_read()->value2, first.value2);
    ASSERT_EQ(locked.lock_read()->value3, first.value3);
    locked.write(value);

    ASSERT_EQ(locked.lock_read()->value1, other.value1);
    ASSERT_EQ(locked.lock_read()->value2, other.value2);
    ASSERT_EQ(locked.lock_read()->value3, other.value3);
}

}
