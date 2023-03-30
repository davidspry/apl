//
// Created by David Spry on 20/1/2023.
//

#pragma once

#include "test_types.hpp"

#include <apl/mwmr_queue.hpp>
#include <benchmark/benchmark.h>
#include <cstddef>
#include <string>

namespace apl::test {

template<typename value_type, std::size_t CAPACITY>
struct mwmr_queue: benchmark::Fixture {
    apl::mwmr_queue<value_type, CAPACITY> queue{};
};

BENCHMARK_TEMPLATE_DEFINE_F(mwmr_queue, push_then_pop_lvalue, detail::pod_type, 16)(benchmark::State& state) {
    const auto value = detail::pod_type();
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(queue.push(value));
        benchmark::DoNotOptimize(queue.pop());
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(mwmr_queue, push_then_pop_rvalue, detail::pod_type, 16)(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(queue.push(detail::pod_type()));
        benchmark::DoNotOptimize(queue.pop());
    }
}

BENCHMARK_REGISTER_F(mwmr_queue, push_then_pop_lvalue)->ThreadRange(1, 8);
BENCHMARK_REGISTER_F(mwmr_queue, push_then_pop_rvalue)->ThreadRange(1, 8);

}
