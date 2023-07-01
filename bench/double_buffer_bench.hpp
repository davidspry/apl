//
// Created by David Spry on 10/5/2023.
//

#pragma once

#include "../test/test_types.hpp"

#include <apl/double_buffer.hpp>
#include <benchmark/benchmark.h>

namespace apl::test {

template<typename T>
struct double_buffer: benchmark::Fixture {
    apl::double_buffer<T> double_buffer{};
};

BENCHMARK_TEMPLATE_F(double_buffer, get, detail::pod_type)(benchmark::State& state) {
    for (auto _: state) {
        benchmark::DoNotOptimize(
                double_buffer.get()
        );
    }
}

BENCHMARK_TEMPLATE_F(double_buffer, set_from_lvalue, detail::pod_type)(benchmark::State& state) {
    const auto value = detail::pod_type{1, 2, "3"};
    for (auto _: state) {
        double_buffer.set(value);
    }
}

BENCHMARK_TEMPLATE_F(double_buffer, set_from_rvalue, detail::pod_type)(benchmark::State& state) {
    for (auto _: state) {
        double_buffer.set(
                detail::pod_type{1, 2, "3"}
        );
    }
}

BENCHMARK_TEMPLATE_F(double_buffer, set_by_write_ptr, detail::pod_type)(benchmark::State& state) {
    for (auto _: state) {
        auto modifier = double_buffer.write_ptr();
        modifier->value1 = 10;
        modifier->value2 = 20;
        modifier->value3 = "30";
    }
}

}
