//
// Created by David Spry on 5/10/2022.
//

#pragma once

#include "../test/test_types.hpp"

#include <apl/array.hpp>
#include <array>
#include <benchmark/benchmark.h>
#include <cstddef>
#include <string>

namespace apl::test {

struct apl_array: benchmark::Fixture {};
struct cpp_array: benchmark::Fixture {};

BENCHMARK_F(apl_array, initialise_default_128)(benchmark::State& state) {
    for (auto _: state) {
        benchmark::DoNotOptimize(
                apl::array<detail::pod_type, 128>{}
        );
    }
}

BENCHMARK_F(cpp_array, initialise_default_128)(benchmark::State& state) {
    for (auto _: state) {
        benchmark::DoNotOptimize(
                std::array<detail::pod_type, 128>{}
        );
    }
}

BENCHMARK_F(apl_array, initialise_with_ctor_arguments_128_copyable)(benchmark::State& state) {
    for (auto _: state) {
        benchmark::DoNotOptimize(
                apl::array<detail::copyable, 128>{1, 2, "3"}
        );
    }
}

BENCHMARK_F(apl_array, initialise_with_ctor_arguments_128_noncopyable)(benchmark::State& state) {
    for (auto _: state) {
        benchmark::DoNotOptimize(
                apl::array<detail::noncopyable, 128>{1, 2, "3"}
        );
    }
}

BENCHMARK_F(apl_array, initialise_with_fill_value_128)(benchmark::State& state) {
    const auto fill_value = detail::copyable(1, 2, "3");
    for (auto _: state) {
        benchmark::DoNotOptimize(
                apl::array<detail::copyable, 128>{fill_value}
        );
    }
}

BENCHMARK_F(cpp_array, initialise_then_fill_value_128)(benchmark::State& state) {
    const auto fill_value = detail::copyable(1, 2, "3");
    for (auto _: state) {
        auto array = std::array<detail::copyable, 128>{};
        array.fill(fill_value);
    }
}

BENCHMARK_F(apl_array, initialise_default_1024)(benchmark::State& state) {
    for (auto _: state) {
        benchmark::DoNotOptimize(
                apl::array<detail::pod_type, 1024>{}
        );
    }
}

BENCHMARK_F(cpp_array, initialise_default_1024)(benchmark::State& state) {
    for (auto _: state) {
        benchmark::DoNotOptimize(
                std::array<detail::pod_type, 1024>{}
        );
    }
}

BENCHMARK_F(apl_array, initialise_with_ctor_arguments_1024_copyable)(benchmark::State& state) {
    for (auto _: state) {
        benchmark::DoNotOptimize(
                apl::array<detail::copyable, 1024>{1, 2, "3"}
        );
    }
}

BENCHMARK_F(apl_array, initialise_with_ctor_arguments_1024_noncopyable)(benchmark::State& state) {
    for (auto _: state) {
        benchmark::DoNotOptimize(
                apl::array<detail::noncopyable, 1024>{1, 2, "3"}
        );
    }
}

BENCHMARK_F(apl_array, initialise_with_fill_value_1024)(benchmark::State& state) {
    const auto fill_value = detail::copyable(1, 2, "3");
    for (auto _: state) {
        benchmark::DoNotOptimize(
                apl::array<detail::copyable, 1024>{fill_value}
        );
    }
}

BENCHMARK_F(cpp_array, initialise_then_fill_value_1024)(benchmark::State& state) {
    const auto fill_value = detail::copyable(1, 2, "3");
    for (auto _: state) {
        auto array = std::array<detail::copyable, 1024>{};
        array.fill(fill_value);
    }
}

}
