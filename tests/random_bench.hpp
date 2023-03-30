//
// Created by David Spry on 20/1/2023.
//

#pragma once

#include "test_types.hpp"

#include <apl/random.hpp>
#include <benchmark/benchmark.h>

namespace apl::test {

struct random: benchmark::Fixture {
    apl::random random{};
};

BENCHMARK_F(random, xorshift)(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                random()
        );
    }
}

BENCHMARK_F(random, integer)(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                random.integer(-2048, 2048)
        );
    }
}

BENCHMARK_F(random, chance)(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                random.chance()
        );
    }
}

BENCHMARK_F(random, real)(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                random.real<double>()
        );
    }
}

BENCHMARK_F(random, real_range)(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                random.real<double>(-2048, 2048)
        );
    }
}

}
