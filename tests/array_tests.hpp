//
// Created by David Spry on 5/10/2022.
//

#pragma once

#include "test_types.hpp"

#include <apl/types/array.hpp>
#include <array>
#include <gtest/gtest.h>
#include <numeric>
#include <span>
#include <vector>

namespace apl {

TEST(array, initialise_default) {
    constexpr static auto array_size = 4;
    constexpr auto array = apl::array<detail::pod_type, array_size>{};

    ASSERT_EQ(array.size(), array_size);

    for (const auto& element: array) {
        ASSERT_EQ(element.value1, 0);
        ASSERT_EQ(element.value2, 0);
        ASSERT_EQ(element.value3, "0");
    }
}

TEST(array, initialise_with_ctor_arguments) {
    constexpr static auto array_size = 4;
    constexpr auto array1 = apl::array<detail::noncopyable, array_size>{1, 2, "3"};
    constexpr auto array2 = apl::array<detail::copyable, array_size>{1, 2, "3"};

    ASSERT_EQ(array1.size(), array_size);
    ASSERT_EQ(array2.size(), array_size);

    for (const auto& element: array1) {
        ASSERT_EQ(element.value1, 1);
        ASSERT_EQ(element.value2, 2);
        ASSERT_EQ(element.value3, "3");
    }

    for (const auto& element: array2) {
        ASSERT_EQ(element.value1, 1);
        ASSERT_EQ(element.value2, 2);
        ASSERT_EQ(element.value3, "3");
    }
}

TEST(array, initialise_with_fill_value) {
    constexpr static auto array_size = 4;
    constexpr auto array = apl::array<detail::copyable, array_size>{detail::copyable{1, 1, "1"}};

    ASSERT_EQ(array.size(), array_size);

    for (const auto& element: array) {
        ASSERT_EQ(element.value1, 1);
        ASSERT_EQ(element.value2, 1);
        ASSERT_EQ(element.value3, "1");
    }
}

TEST(array, initialise_with_elements) {
    constexpr static auto string_view = std::string_view("i");
    const static std::string string = "!";

    constexpr static auto array_size = 5;
    constexpr auto array1 = apl::array<int, array_size>::prefixing(1, 2, 3, 4, 5);
    constexpr auto array2 = apl::array<int, array_size>::repeating(1, 2);
    constexpr auto array3 = apl::array<std::string_view, array_size>::repeating(string_view, std::string_view("i"));
    const auto array4 = apl::array<std::string, array_size>::repeating(string, string, std::string("!"));

    ASSERT_EQ(array1.size(), array_size);
    ASSERT_EQ(array2.size(), array_size);
    ASSERT_EQ(array3.size(), array_size);
    ASSERT_EQ(array4.size(), array_size);

    for (auto i = 0; i < array_size; ++i) {
        ASSERT_EQ(array1[i], i + 1);
        ASSERT_EQ(array2[i], i % 2 + 1);
        ASSERT_EQ(array3[i], "i");
        ASSERT_EQ(array4[i], string);
    }
}

TEST(array, initialise_from_static_array) {
    constexpr static auto array_size = 4;
    constexpr auto array1 = apl::array<detail::copyable, array_size>{};
    constexpr auto array2 = array{array1};
    ASSERT_EQ(array1, array2);
}

TEST(array, initialise_from_static_array_mismatching_size) {
    constexpr static auto array_size = 4;
    constexpr auto large_source = apl::array<detail::copyable, (array_size * 2)>{};
    constexpr auto small_source = apl::array<detail::copyable, (array_size / 2)>{};
    constexpr auto array1 = apl::array<detail::copyable, array_size>{large_source};
    constexpr auto array2 = apl::array<detail::copyable, array_size>{small_source};

    ASSERT_EQ(array1.size(), array_size);
    ASSERT_EQ(array2.size(), array_size);

    for (auto i = 0; i < array_size; ++i) {
        ASSERT_EQ(array1[i], large_source[i]);
    }

    for (auto i = 0; i < array_size / 2; ++i) {
        ASSERT_EQ(array2[i], small_source[i]);
    }

    for (auto i = array_size / 2; i < array_size; ++i) {
        ASSERT_EQ(array2[i], detail::copyable{});
    }
}

TEST(array, initialise_from_array) {
    constexpr static auto array_size = 4;
    constexpr auto source = std::array<detail::copyable, array_size>{};
    constexpr auto array1 = array{source};
    constexpr auto array2 = apl::array<detail::copyable, array_size>::prefixing(source);
    constexpr auto array3 = apl::array<detail::copyable, array_size>::repeating(source);

    ASSERT_EQ(source.size(), array1.size());
    ASSERT_EQ(source.size(), array2.size());
    ASSERT_EQ(source.size(), array3.size());

    for (auto i = 0; i < array_size; ++i) {
        ASSERT_EQ(source[i].value1, array1[i].value1);
        ASSERT_EQ(source[i].value2, array1[i].value2);
        ASSERT_EQ(source[i].value3, array1[i].value3);
    }

    for (auto i = 0; i < array_size; ++i) {
        ASSERT_EQ(source[i].value1, array2[i].value1);
        ASSERT_EQ(source[i].value2, array2[i].value2);
        ASSERT_EQ(source[i].value3, array2[i].value3);
    }

    for (auto i = 0; i < array_size; ++i) {
        ASSERT_EQ(source[i].value1, array3[i].value1);
        ASSERT_EQ(source[i].value2, array3[i].value2);
        ASSERT_EQ(source[i].value3, array3[i].value3);
    }
}

TEST(array, initialise_from_array_mismatching_size) {
    constexpr static auto array_size = 2;
    constexpr auto large_source = std::array<int, (array_size * 2)>{1, 2, 3, 4};
    constexpr auto small_source = std::array<int, (array_size / 2)>{1};
    constexpr auto array1 = apl::array<int, array_size>{large_source};
    constexpr auto array2 = apl::array<int, array_size>{small_source};

    ASSERT_EQ(array1.size(), array_size);
    ASSERT_EQ(array2.size(), array_size);

    for (auto i = 0; i < array_size; ++i) {
        ASSERT_EQ(array1[i], large_source[i]);
    }

    for (auto i = 0; i < small_source.size(); ++i) {
        ASSERT_EQ(array2[i], small_source[i]);
    }

    for (auto i = small_source.size(); i < array_size; ++i) {
        ASSERT_EQ(array2[i], 0);
    }
}

TEST(array, initialise_from_span_mismatching_size) {
    constexpr static auto array_size = 4;
    const auto large_vector = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8};
    const auto small_vector = std::vector<int>{1, 2};
    const auto large_span = std::span(large_vector);
    const auto small_span = std::span(small_vector);

    //! from large span
    const auto large_array1 = apl::array<int, array_size>::prefixing(large_span);
    const auto large_array2 = apl::array<int, array_size>::repeating(large_span);

    ASSERT_EQ(large_array1.size(), array_size);
    ASSERT_EQ(large_array2.size(), array_size);

    for (auto i = 0; i < array_size; ++i) {
        ASSERT_EQ(large_array1[i], large_vector[i]);
        ASSERT_EQ(large_array2[i], large_vector[i]);
    }

    //! from small span
    const auto small_array1 = apl::array<int, array_size>::prefixing(small_span);
    const auto small_array2 = apl::array<int, array_size>::repeating(small_span);

    ASSERT_EQ(small_array1.size(), array_size);
    ASSERT_EQ(small_array2.size(), array_size);

    for (auto i = 0; i < array_size / 2; ++i) {
        ASSERT_EQ(small_array1[i], small_vector[i]);
        ASSERT_EQ(small_array2[i], small_vector[i]);
    }

    for (auto i = array_size / 2; i < array_size; ++i) {
        ASSERT_EQ(small_array1[i], int{});
        ASSERT_EQ(small_array2[i], small_vector[i % (array_size / 2)]);
    }
}

TEST(array, element_access) {
    constexpr static auto array_size = 4;
    constexpr auto const_array = apl::array<int, array_size>::repeating(1, 2, 3, 4);
    auto mutable_array = apl::array<int, array_size>::repeating(1, 2, 3, 4);

    ASSERT_EQ(array_size, const_array.size());
    ASSERT_EQ(array_size, mutable_array.size());

    for (auto i = 0; i < array_size; ++i) {
        const auto expected_value = i % array_size + 1;
        ASSERT_EQ(const_array[i], expected_value);
        ASSERT_EQ(mutable_array[i], expected_value);
        ASSERT_EQ(const_array.at(i), expected_value);
        ASSERT_EQ(mutable_array.at(i), expected_value);
    }
}

TEST(array, fill) {
    constexpr static auto array_size = 4;
    auto array = apl::array<int, array_size>{1};

    for (const auto element: array) {
        ASSERT_EQ(element, 1);
    }

    array.fill(2);
    for (const auto element: array) {
        ASSERT_EQ(element, 2);
    }
}

TEST(array, size) {
    constexpr static auto array_size = 8;
    constexpr auto array = apl::array<std::byte, array_size>{};

    ASSERT_EQ(array.size(), array_size);
    ASSERT_EQ(array.size(), sizeof(array));
}

TEST(array, operator_plus) {
    constexpr static auto array_size = 4;
    constexpr auto array1 = apl::array<std::string_view, array_size>::repeating(std::string_view("a"));
    constexpr auto array2 = apl::array<std::string_view, array_size>::repeating(std::string_view("b"));
    constexpr auto array3 = array1 + array2;

    ASSERT_EQ(array1.size(), array_size);
    ASSERT_EQ(array1.size(), array2.size());
    ASSERT_EQ(array3.size(), array_size * 2);

    for (auto i = 0; i < array_size; ++i) {
        ASSERT_EQ(array3[i], "a");
    }

    for (auto i = array_size; i < array_size * 2; ++i) {
        ASSERT_EQ(array3[i], "b");
    }
}

TEST(array, operator_equality) {
    constexpr static auto array_size = 4;
    constexpr auto source = apl::array<std::string_view, array_size>::repeating(std::string_view("a"));
    ASSERT_EQ(source, source);

    constexpr auto other_size = array_size + 1;
    constexpr auto mismatching_size = apl::array<std::string_view, other_size>::repeating(std::string_view("a"));
    constexpr auto mismatching_data = apl::array<std::string_view, array_size>::repeating(std::string_view("b"));
    ASSERT_NE(source, mismatching_size);
    ASSERT_NE(source, mismatching_data);
}

TEST(array, forward_iterator) {
    constexpr static auto array_size = 5;
    constexpr auto array = apl::array<int, array_size>::prefixing(1, 2, 3, 4, 5);

    for (const auto element: array) {
        static auto expected_value = 1;
        ASSERT_EQ(element, expected_value++);
    }

    ASSERT_EQ(1 + 2 + 3 + 4 + 5, std::accumulate(array.begin(), array.end(), 0));
}

TEST(array, reverse_iterator) {
    constexpr static auto array_size = 5;
    constexpr auto array = apl::array<int, array_size>::prefixing(1, 2, 3, 4, 5);

    for (auto i = array.rbegin(); i != array.rend(); ++i) {
        static auto expected_value = array_size;
        ASSERT_EQ(*i, expected_value--);
    }

    ASSERT_EQ(5 + 4 + 3 + 2 + 1, std::accumulate(array.rbegin(), array.rend(), 0));
}

}
