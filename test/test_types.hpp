//
// Created by David Spry on 16/10/2022.
//

#pragma once

#include <string_view>

namespace apl::detail {
struct pod_type {
    int value1{0};
    int value2{0};
    std::string_view value3{"0"};
    constexpr bool operator==(const pod_type&) const = default;
};

struct copyable {
    int value1{0};
    int value2{0};
    std::string_view value3{"0"};

    constexpr copyable() = default;
    constexpr copyable(const int value1,
                       const int value2,
                       const std::string_view value3):
            value1(value1),
            value2(value2),
            value3(value3) {
    }
    constexpr bool operator==(const copyable&) const = default;
};

struct noncopyable {
    int value1{0};
    int value2{0};
    std::string_view value3{"0"};

    constexpr noncopyable() = default;
    constexpr noncopyable(const int value1,
                          const int value2,
                          const std::string_view value3):
            value1(value1),
            value2(value2),
            value3(value3) {
    }
    noncopyable(noncopyable&&) = default;
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(noncopyable&&) = default;
    noncopyable& operator=(const noncopyable&) = delete;
    constexpr bool operator==(const noncopyable&) const = default;
};
}
