//
// Created by David Spry on 30/3/2023.
//

#pragma once

#include <apl_events/include/notify.hpp>
#include <atomic>
#include <cstddef>
#include <gtest/gtest.h>
#include <string_view>
#include <type_traits>

namespace apl::test {

namespace notify {
enum class notifications { type1, type2, type3 };

struct state_t {
  using atomic = std::false_type;
  using value_type = std::string_view;
};

struct receiver_t {
  std::size_t enum_count;
  std::size_t type_count;

  template<auto notification> auto notify() {
    if constexpr (std::same_as<decltype(notification), notifications>) {
      enum_count += 1;
    }
  }

  template<typename state_type> auto notify() {
    if constexpr (std::same_as<state_type, state_t>) {
      type_count += 1;
    }
  }
};
}

TEST(notify, state_type) {
  auto r1 = notify::receiver_t{};
  auto r2 = notify::receiver_t{};
  auto r3 = notify::receiver_t{};

  for (auto i = 0; i < 32; ++i) {
    apl::notify<notify::state_t>(r3, r2, r1);
    apl::notify<notify::state_t>(r3, r2);
    apl::notify<notify::state_t>(r3);

    ASSERT_EQ(r1.type_count, (i + 1) * 1);
    ASSERT_EQ(r2.type_count, (i + 1) * 2);
    ASSERT_EQ(r3.type_count, (i + 1) * 3);
  }
}

TEST(notify, enum_type) {
  auto r1 = notify::receiver_t{};
  auto r2 = notify::receiver_t{};
  auto r3 = notify::receiver_t{};

  for (auto i = 0; i < 32; ++i) {
    apl::notify<notify::notifications::type1>(r1);
    apl::notify<notify::notifications::type2>(r2, r2);
    apl::notify<notify::notifications::type3>(r3, r3, r3);

    ASSERT_EQ(r1.enum_count, (i + 1) * 1);
    ASSERT_EQ(r2.enum_count, (i + 1) * 2);
    ASSERT_EQ(r3.enum_count, (i + 1) * 3);
  }
}

}
