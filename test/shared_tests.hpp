//
// Created by David Spry on 30/3/2023.
//

#pragma once

#include "test_types.hpp"

#include <apl_events/include/shared.hpp>
#include <atomic>
#include <cstddef>
#include <gtest/gtest.h>
#include <string_view>
#include <type_traits>

namespace apl::test {

namespace shared {
struct state_t {
    using atomic = std::false_type;
    using value_type = std::size_t;
};

struct atomic_state_t {
    using atomic = std::true_type;
    using value_type = std::size_t;
};

struct receiver_t {
    std::size_t did_set_count;
    state_t::value_type received_value;
    template<auto notification> auto notify() {
        if constexpr (apl::shared<shared::state_t>::did_set<notification>()) {
            did_set_count += 1;
            received_value = apl::shared<state_t>::get();
        }
    }
};
}

TEST(shared, initial_value) {
    ASSERT_EQ(shared::state_t::value_type{}, apl::shared<shared::state_t>::get());
    ASSERT_EQ(shared::state_t::value_type{}, apl::shared<shared::atomic_state_t>::get());
}

TEST(shared, set_get_value) {
    for (const auto [value1, value2]: std::array{
            std::make_tuple(16uz, 32uz),
            std::make_tuple(24uz, 48uz),
            std::make_tuple(32uz, 64uz),
            std::make_tuple(40uz, 80uz),
            std::make_tuple(48uz, 96uz)
    }) {
        apl::shared<shared::state_t>::set(value1);
        apl::shared<shared::atomic_state_t>::set(value2);

        ASSERT_EQ(value1, apl::shared<shared::state_t>::get());
        ASSERT_EQ(value2, apl::shared<shared::atomic_state_t>::get());
    }
}

TEST(shared, set_notify) {
    auto r1 = shared::receiver_t{};
    auto r2 = shared::receiver_t{};
    for (const auto [value1, value2]: std::array{
            std::make_tuple(1uz, 10uz),
            std::make_tuple(2uz, 20uz),
            std::make_tuple(3uz, 30uz),
            std::make_tuple(4uz, 40uz),
            std::make_tuple(5uz, 50uz)
    }) {
        apl::shared<shared::state_t>::set_notify(value1)(r1, r2);
        apl::shared<shared::state_t>::set_notify(value2)(r2);

        ASSERT_EQ(r1.received_value, value1);
        ASSERT_EQ(r2.received_value, value2);
    }

    ASSERT_EQ(r1.did_set_count, 5);
    ASSERT_EQ(r2.did_set_count, 10);
}

}
