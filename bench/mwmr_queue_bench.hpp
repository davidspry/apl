//
// Created by David Spry on 20/1/2023.
//

#pragma once

#include "../test/test_types.hpp"

#include <apl_threading/include/mwmr_queue.hpp>
#include <benchmark/benchmark.h>
#include <cstddef>
#include <string>
#include <thread>

namespace apl::test {

template<typename T, std::size_t CAPACITY>
struct mwmr_queue: benchmark::Fixture {
  apl::mwmr_queue<T, CAPACITY> queue{};
};

BENCHMARK_TEMPLATE_DEFINE_F(mwmr_queue, push_then_pop_lvalue, detail::pod_type, 16)(benchmark::State& state) {
  const auto value = detail::pod_type();
  for (auto _: state) {
    benchmark::DoNotOptimize(queue.push(value));
    benchmark::DoNotOptimize(queue.pop());
  }
}

BENCHMARK_REGISTER_F(mwmr_queue, push_then_pop_lvalue)->ThreadRange(1, static_cast<int>(std::thread::hardware_concurrency()));

BENCHMARK_TEMPLATE_DEFINE_F(mwmr_queue, push_then_pop_rvalue, detail::pod_type, 16)(benchmark::State& state) {
  for (auto _: state) {
    benchmark::DoNotOptimize(queue.push(detail::pod_type()));
    benchmark::DoNotOptimize(queue.pop());
  }
}

BENCHMARK_REGISTER_F(mwmr_queue, push_then_pop_rvalue)->ThreadRange(1, static_cast<int>(std::thread::hardware_concurrency()));

}
