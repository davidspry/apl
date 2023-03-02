//
// Created by David Spry on 20/1/2023.
//

#pragma once

#include "test_types.hpp"

#include <apl/concurrency/seqlock.hpp>
#include <benchmark/benchmark.h>
#include <cstddef>
#include <string>

namespace apl::test {

template<typename value_type>
struct seqlock: benchmark::Fixture {
    apl::seqlock<value_type> seqlock{};
};

BENCHMARK_TEMPLATE_F(seqlock, store_lvalue, detail::pod_type)(benchmark::State& state) {
    const auto value = detail::pod_type();
    while (state.KeepRunning()) {
        seqlock.store(value);
    }
}

BENCHMARK_TEMPLATE_F(seqlock, store_rvalue, detail::pod_type)(benchmark::State& state) {
    while (state.KeepRunning()) {
        seqlock.store(detail::pod_type());
    }
}

BENCHMARK_TEMPLATE_F(seqlock, read, detail::pod_type)(benchmark::State& state) {
    auto loaded_value = detail::pod_type{};
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(seqlock.load(loaded_value));
    }
}

}
