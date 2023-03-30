//
// Created by David Spry on 29/1/2023.
//

#pragma once

#include "test_types.hpp"

#include <apl/mod.hpp>
#include <benchmark/benchmark.h>
#include <concepts>

namespace apl::test {

template<std::size_t LIMIT, std::unsigned_integral unsigned_integral_type>
struct mod: benchmark::Fixture {
    apl::mod<LIMIT, unsigned_integral_type> mod;
};

BENCHMARK_TEMPLATE_F(mod, addition, 128, std::uint64_t)(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                mod + 3u
        );
    }
}

BENCHMARK_TEMPLATE_F(mod, addition_assignment, 128, std::uint64_t)(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                mod += 3u
        );
    }
}

BENCHMARK_TEMPLATE_F(mod, incremented_by, 128, std::uint64_t)(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                mod.incremented_by(3u)
        );
    }
}

BENCHMARK_TEMPLATE_F(mod, subtraction, 128, std::uint64_t)(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                mod - 3u
        );
    }
}

BENCHMARK_TEMPLATE_F(mod, subtraction_assignment, 128, std::uint64_t)(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                mod -= 3u
        );
    }
}

BENCHMARK_TEMPLATE_F(mod, decremented_by, 128, std::uint64_t)(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                mod.decremented_by(3u)
        );
    }
}

BENCHMARK_TEMPLATE_F(mod, multiplication, 128, std::uint64_t)(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                mod * 3u
        );
    }
}

BENCHMARK_TEMPLATE_F(mod, multiplication_assignment, 128, std::uint64_t)(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                mod *= 3u
        );
    }
}

BENCHMARK_TEMPLATE_F(mod, incremented64, 128, std::uint64_t)(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                mod.incremented()
        );
    }
}

BENCHMARK_TEMPLATE_F(mod, decremented64, 128, std::uint64_t)(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                mod.decremented()
        );
    }
}

BENCHMARK_TEMPLATE_F(mod, incremented32, 128, std::uint32_t)(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                mod.incremented()
        );
    }
}

BENCHMARK_TEMPLATE_F(mod, decremented32, 128, std::uint32_t)(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                mod.decremented()
        );
    }
}

BENCHMARK_TEMPLATE_F(mod, incremented8, 128, std::uint8_t)(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                mod.incremented()
        );
    }
}

BENCHMARK_TEMPLATE_F(mod, decremented8, 128, std::uint8_t)(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                mod.decremented()
        );
    }
}

}
