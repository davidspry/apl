//
// Created by David Spry on 22/1/2023.
//

#pragma once

#include "test_types.hpp"

#include <apl/types/random.hpp>
#include <cstddef>
#include <gtest/gtest.h>
#include <ranges>

namespace apl::test {

TEST(random, initialise_from_same_seed) {
    for (auto i = 0uz; i < 128z; ++i) {
        const auto seed = i << 1uz;
        const auto random1 = apl::random{seed};
        const auto random2 = apl::random{seed};
        for (auto k = 0; k < 32uz; ++k) {
            ASSERT_EQ(
                    std::make_tuple(
                            random1(),
                            random1.integer(0, 1024),
                            random1.chance(),
                            random1.real<double>(),
                            random1.real<double>(1024.0, 2048.0)
                    ),
                    std::make_tuple(
                            random2(),
                            random2.integer(0, 1024),
                            random2.chance(),
                            random2.real<double>(),
                            random2.real<double>(1024.0, 2048.0)
                    )
            );
        }
    }
}

TEST(random, initialise_from_different_seeds) {
    constexpr auto indices = std::make_index_sequence<16uz>{};
    for (auto i = 0uz; i < 128uz; ++i) {
        const auto random1 = apl::random{i + 1};
        const auto random2 = apl::random{i + 2};
        for (auto i = 0uz; i < 32uz; ++i) {
            ASSERT_NE(
                    std::make_tuple(
                            random1(),
                            random1.integer(0, 1024),
                            random1.chance(),
                            random1.real<double>(),
                            random1.real<double>(1024.0, 2048.0)
                    ),
                    std::make_tuple(
                            random2(),
                            random2.integer(0, 1024),
                            random2.chance(),
                            random2.real<double>(),
                            random2.real<double>(1024.0, 2048.0)
                    )
            );
        }
    }
}

}
