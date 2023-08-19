//
// Created by David Spry on 4/12/2022.
//

#pragma once

#include "test_types.hpp"

#include <apl_threading/include/thread_group.hpp>
#include <atomic>
#include <gtest/gtest.h>
#include <thread>

namespace apl {

TEST(thread_group, initialise_default) {
    const auto thread_group = apl::thread_group<1>{};
    ASSERT_FALSE(thread_group.is_busy());
}

TEST(thread_group, submit_task) {
    static constexpr auto new_task = []() {};
    auto thread_group = apl::thread_group<1, 256>{};
    thread_group.submit_task(new_task);
    thread_group.submit_task(new_task);
    thread_group.submit_task(new_task);
    ASSERT_TRUE(thread_group.is_busy());
}

TEST(thread_group, perform_tasks) {
    std::atomic<std::size_t> count{0};
    auto thread_group = apl::thread_group<4, 0>{};

    static constexpr auto desired_count = 4;
    for (auto i = 0; i < desired_count; ++i) {
        if (!thread_group.submit_task([&count]() {
            count.fetch_add(1);
        })) {
            FAIL();
        }
    }

    while (thread_group.is_busy()) {
        std::this_thread::sleep_for(
                std::chrono::milliseconds(1)
        );
    }

    ASSERT_EQ(count.load(), desired_count);
}

}
