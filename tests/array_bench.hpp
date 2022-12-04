//
// Created by David Spry on 5/10/2022.
//

#pragma once

#include "test_types.hpp"

#include <apl/types/array.hpp>
#include <array>
#include <benchmark/benchmark.h>
#include <cstddef>
#include <string>

namespace apl {

static auto apl_array_initialise_default_128(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                apl::array<detail::pod_type, 128> {}
        );
    }
}

BENCHMARK(apl_array_initialise_default_128);

static auto cpp_array_initialise_default_128(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                std::array<detail::pod_type, 128> {}
        );
    }
}

BENCHMARK(cpp_array_initialise_default_128);

static auto apl_array_initialise_with_ctor_arguments_128_copyable(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                apl::array<detail::copyable, 128> {1, 2, "3"}
        );
    }
}

BENCHMARK(apl_array_initialise_with_ctor_arguments_128_copyable);

static auto apl_array_initialise_with_ctor_arguments_128_noncopyable(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                apl::array<detail::noncopyable, 128> {1, 2, "3"}
        );
    }
}

BENCHMARK(apl_array_initialise_with_ctor_arguments_128_noncopyable);

static auto apl_array_initialise_with_fill_value_128(benchmark::State& state) {
    const auto fill_value = detail::copyable(1, 2, "3");
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                apl::array<detail::copyable, 128> {fill_value}
        );
    }
}

BENCHMARK(apl_array_initialise_with_fill_value_128);

static auto cpp_array_initialise_then_fill_value_128(benchmark::State& state) {
    const auto fill_value = detail::copyable(1, 2, "3");
    while (state.KeepRunning()) {
        auto array = std::array<detail::copyable, 128>{};
        array.fill(fill_value);
    }
}

BENCHMARK(cpp_array_initialise_then_fill_value_128);

static auto apl_array_initialise_default_1024(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                apl::array<detail::pod_type, 1024> {}
        );
    }
}

BENCHMARK(apl_array_initialise_default_1024);

static auto cpp_array_initialise_default_1024(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                std::array<detail::pod_type, 1024> {}
        );
    }
}

BENCHMARK(cpp_array_initialise_default_1024);

static auto apl_array_initialise_with_ctor_arguments_1024_copyable(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                apl::array<detail::copyable, 1024> {1, 2, "3"}
        );
    }
}

BENCHMARK(apl_array_initialise_with_ctor_arguments_1024_copyable);

static auto apl_array_initialise_with_ctor_arguments_1024_noncopyable(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                apl::array<detail::noncopyable, 1024> {1, 2, "3"}
        );
    }
}

BENCHMARK(apl_array_initialise_with_ctor_arguments_1024_noncopyable);

static auto apl_array_initialise_with_fill_value_1024(benchmark::State& state) {
    const auto fill_value = detail::copyable(1, 2, "3");
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                apl::array<detail::copyable, 1024> {fill_value}
        );
    }
}

BENCHMARK(apl_array_initialise_with_fill_value_1024);

static auto cpp_array_initialise_then_fill_value_1024(benchmark::State& state) {
    const auto fill_value = detail::copyable(1, 2, "3");
    while (state.KeepRunning()) {
        auto array = std::array<detail::copyable, 1024>{};
        array.fill(fill_value);
    }
}

BENCHMARK(cpp_array_initialise_then_fill_value_1024);

}
