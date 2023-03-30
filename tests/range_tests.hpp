//
// Created by David Spry on 25/1/2023.
//

#pragma once

#include "test_types.hpp"

#include <apl/range.hpp>
#include <fmt/format.h>
#include <gtest/gtest.h>
#include <numeric>

namespace apl::test {

TEST(range, size) {
    ASSERT_EQ((range<0, 1>::size()), 1);
    ASSERT_EQ((range<0, 4>::size()), 4);
    ASSERT_EQ((range<-4, +4>::size()), 8);
    ASSERT_EQ((range<-4, -3>::size()), 1);
}

TEST(range, contains) {
    ASSERT_FALSE((range<1, 3>::contains(0)));
    ASSERT_TRUE ((range<1, 3>::contains(1)));
    ASSERT_TRUE ((range<1, 3>::contains(2)));
    ASSERT_FALSE((range<0, 3>::contains(3)));
    ASSERT_TRUE ((range<-5192, 5192>::contains(0)));
    ASSERT_FALSE((range<-5192, 5192>::contains(10'384)));
}

TEST(range, get) {
    using range_type = range<-5192, 5192>;
    ASSERT_EQ((range_type::get<0>()), range_type::lower_bound());
    ASSERT_EQ((range_type::get<32>()), range_type::lower_bound() + 32);
    ASSERT_EQ((range_type::get<64>()), range_type::lower_bound() + 64);
    ASSERT_EQ((range_type::get<128>()), range_type::lower_bound() + 128);
    ASSERT_EQ((range_type::get<range_type::size() - 1>()), range_type::upper_bound() - 1);
}

TEST(range, forwards_iteration) {
    constexpr auto range = apl::range<1, 6>{};
    constexpr auto array = std::array{1, 2, 3, 4, 5};
    auto index = 0;

    for (const auto number: range) {
        ASSERT_EQ(
                number,
                array.at(index++)
        );
    }

    constexpr auto range_sum = std::accumulate(range.begin(), range.end(), 0ll);
    constexpr auto array_sum = std::accumulate(array.begin(), array.end(), 0);

    ASSERT_EQ(range_sum, array_sum);
}

TEST(range, forwards_iteration_step_by_2) {
    constexpr auto range = apl::range<1, 10>::step_by<2>{};
    constexpr auto array = std::array{1, 3, 5, 7, 9};
    auto index = 0;

    for (const auto number: range) {
        ASSERT_EQ(
                number,
                array.at(index++)
        );
    }

    constexpr auto range_sum = std::accumulate(range.begin(), range.end(), 0ll);
    constexpr auto array_sum = std::accumulate(array.begin(), array.end(), 0);
    ASSERT_EQ(range_sum, array_sum);
}

TEST(range, reverse_iteration) {
    constexpr auto range = apl::range<1, 6>::reversed{};
    constexpr auto array = std::array{5, 4, 3, 2, 1};
    auto index = 0;

    for (const auto number: range) {
        ASSERT_EQ(
                number,
                array.at(index++)
        );
    }

    constexpr auto range_sum = std::accumulate(range.begin(), range.end(), 0ll);
    constexpr auto array_sum = std::accumulate(array.begin(), array.end(), 0);
    ASSERT_EQ(range_sum, array_sum);
}

TEST(range, reverse_iteration_step_by_2) {
    constexpr auto range = apl::range<1, 10>::reversed::step_by<2>{};
    constexpr auto array = std::array{9, 7, 5, 3, 1};
    auto index = 0;

    for (const auto number: range) {
        ASSERT_EQ(
                number,
                array.at(index++)
        );
    }

    constexpr auto range_sum = std::accumulate(range.begin(), range.end(), 0ll);
    constexpr auto array_sum = std::accumulate(array.begin(), array.end(), 0);
    ASSERT_EQ(range_sum, array_sum);
}

TEST(range, prefix) {
    using range_type = apl::range<-128, 128>;

    //! Prefix
    ASSERT_EQ((range_type::prefix<1>()), (apl::range<range_type::lower_bound(), range_type::lower_bound() + 1>()));
    ASSERT_EQ((range_type::prefix<8>()), (apl::range<range_type::lower_bound(), range_type::lower_bound() + 8>()));
    ASSERT_EQ((range_type::prefix<range_type::size() - 1>()),
              (apl::range<range_type::lower_bound(), range_type::upper_bound() - 1>{}));

    //! Drop-suffix
    ASSERT_EQ((range_type::drop_suffix<1>()), (apl::range<range_type::lower_bound(), range_type::upper_bound() - 1>{}));
    ASSERT_EQ((range_type::drop_suffix<8>()), (apl::range<range_type::lower_bound(), range_type::upper_bound() - 8>{}));
    ASSERT_EQ((range_type::drop_suffix<range_type::size() - 1>()),
              (apl::range<range_type::lower_bound(), range_type::lower_bound() + 1>()));
}

TEST(range, suffix) {
    using range_type = apl::range<-128, 128>;

    //! Suffix
    ASSERT_EQ((range_type::suffix<1>()), (apl::range<range_type::upper_bound() - 1, range_type::upper_bound()>()));
    ASSERT_EQ((range_type::suffix<8>()), (apl::range<range_type::upper_bound() - 8, range_type::upper_bound()>()));
    ASSERT_EQ((range_type::suffix<range_type::size() - 1>()),
              (apl::range<range_type::lower_bound() + 1, range_type::upper_bound()>{}));

    //! Drop-prefix
    ASSERT_EQ((range_type::drop_prefix<1>()), (apl::range<range_type::lower_bound() + 1, range_type::upper_bound()>{}));
    ASSERT_EQ((range_type::drop_prefix<8>()), (apl::range<range_type::lower_bound() + 8, range_type::upper_bound()>{}));
    ASSERT_EQ((range_type::drop_prefix<range_type::size() - 1>()),
              (apl::range<range_type::upper_bound() - 1, range_type::upper_bound()>()));
}

TEST(range, range_builder_reversed_step_by_order) {
    using range_type = apl::range<-16, 16>;

    ASSERT_EQ(
            (range_type::step_by<3>::reversed()),
            (range_type::reversed::step_by<3>())
    );

    ASSERT_EQ(
            (range_type::reversed()),
            (range_type::reversed::reversed::reversed())
    );

    ASSERT_EQ(
            (range_type::reversed::reversed()),
            (range_type())
    );
}

}
