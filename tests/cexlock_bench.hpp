//
// Created by David Spry on 20/1/2023.
//

#pragma once

#include "test_types.hpp"

#include <apl/concurrency/cexlock.hpp>
#include <benchmark/benchmark.h>

namespace apl::test {

template<typename value_type>
struct cexlock: benchmark::Fixture {
    apl::cexlock<value_type> cexlock{};
};

BENCHMARK_TEMPLATE_F(cexlock, lock_read, detail::pod_type)(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
                cexlock.lock_read().operator*()
        );
    }
}

BENCHMARK_TEMPLATE_F(cexlock, write, detail::pod_type)(benchmark::State& state) {
    const auto value = detail::pod_type{1, 2, "3"};
    while (state.KeepRunning()) {
        cexlock.write(value);
    }
}

BENCHMARK_TEMPLATE_F(cexlock, copy_read_then_write, detail::pod_type)(benchmark::State& state) {
    while (state.KeepRunning()) {
        cexlock.write(
                cexlock.copy_read()
        );
    }
}

}
