//
// Created by David Spry on 4/12/2022.
//

#pragma once

#include "test_types.hpp"

#include <apl/concurrency/thread_group.hpp>
#include <atomic>
#include <gtest/gtest.h>
#include <thread>

namespace apl {

TEST(thread_group, initialise_default) {
    const auto thread_group = apl::thread_group<1>{};
    ASSERT_FALSE(thread_group.has_pending_tasks());
}

TEST(thread_group, submit_task) {
    constexpr static auto new_task = []() {};
    auto thread_group = apl::thread_group<1, 250>{};
    thread_group.submit_task(new_task);
    thread_group.submit_task(new_task);
    ASSERT_TRUE(thread_group.has_pending_tasks());
}

TEST(thread_group, perform_tasks) {
    std::atomic<std::size_t> count{0};
    auto add_one = [&count]() { count.fetch_add(1); };
    auto thread_group = apl::thread_group<4, 5>{};

    constexpr static auto desired_count = 4;
    for (auto i = 0; i < desired_count; ++i) {
        thread_group.submit_task(add_one);
    }

    while (thread_group.has_active_tasks() or
           thread_group.has_pending_tasks()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    ASSERT_EQ(count.load(), desired_count);
}

}