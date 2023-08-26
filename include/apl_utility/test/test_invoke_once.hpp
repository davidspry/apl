//! apl
//! David Spry
//! Created on 26/8/2023

#pragma once

#include <apl_threading/include/thread_group.hpp>
#include <apl_utility/include/invoke_once.hpp>
#include <atomic>
#include <cstddef>
#include <gtest/gtest.h>

namespace apl::test {

TEST(invoke_once, single_thread) {
  auto invocation_index = std::size_t{0};

  for (auto i = 0; i < 8; ++i) {
    apl::invoke_once([&invocation_index] {
      invocation_index += 1;
    });
  }

  ASSERT_EQ(invocation_index, 1uz);
}

TEST(invoke_once, multiple_threads) {
  static constexpr auto thread_count = 8uz;
  auto thread_group = apl::thread_group<thread_count, 4>{};
  auto invoke_count = std::atomic{0u};
  auto invoke_tries = std::atomic{0u};

  for (auto i = 0; i < thread_count; ++i) {
    thread_group.submit_task([&invoke_count, &invoke_tries] {
      invoke_tries.fetch_add(1);
      apl::invoke_once([&invoke_count] {
        invoke_count.fetch_add(1);
      });
    });
  }

  while (thread_group.is_busy()) {
    std::this_thread::sleep_for(
      std::chrono::milliseconds(1)
    );
  }

  ASSERT_EQ(invoke_count, 1uz);
  ASSERT_EQ(invoke_tries, thread_count);
}

}
