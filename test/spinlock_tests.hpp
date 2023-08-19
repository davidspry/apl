#pragma once

#include "test_types.hpp"

#include <apl_threading/include/spinlock.hpp>
#include <gtest/gtest.h>

namespace apl::test {

TEST(scoped_spinlock, raii) {
    auto spinlock = apl::spinlock();

    {
        auto ssl = apl::scoped_spinlock(spinlock);
        ASSERT_TRUE(spinlock.is_locked());
        ASSERT_FALSE(spinlock.try_lock());
        ASSERT_FALSE(spinlock.try_lock());
        ASSERT_FALSE(spinlock.try_lock());
    }

    ASSERT_FALSE(spinlock.is_locked());
}

TEST(spinlock, initialise) {
    auto spinlock = apl::spinlock();
    ASSERT_FALSE(spinlock.is_locked());
}

TEST(spinlock, lock) {
    auto spinlock = apl::spinlock();

    spinlock.lock();
    ASSERT_TRUE(spinlock.is_locked());
    ASSERT_FALSE(spinlock.try_lock());
    ASSERT_FALSE(spinlock.try_lock());
    ASSERT_FALSE(spinlock.try_lock());
    spinlock.unlock();

    ASSERT_FALSE(spinlock.is_locked());
}

TEST(spinlock, try_lock) {
    auto spinlock = apl::spinlock();

    static const auto try_lock_from_locked = [&] {
        ASSERT_TRUE(spinlock.is_locked());
        ASSERT_FALSE(spinlock.try_lock());
        ASSERT_FALSE(spinlock.try_lock());
        ASSERT_FALSE(spinlock.try_lock());
        ASSERT_TRUE(spinlock.is_locked());
    };

    static const auto try_lock_from_unlocked = [&] {
        ASSERT_FALSE(spinlock.is_locked());
        ASSERT_TRUE(spinlock.try_lock());
        ASSERT_FALSE(spinlock.try_lock());
        ASSERT_FALSE(spinlock.try_lock());
        ASSERT_TRUE(spinlock.is_locked());
    };

    try_lock_from_unlocked();
    try_lock_from_locked();
    spinlock.unlock();
    try_lock_from_unlocked();
    try_lock_from_locked();
}

}
