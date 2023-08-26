//
// Created by David Spry on 20/1/2023.
//

#pragma once

#include "../test/test_types.hpp"

#include <apl_threading/include/seqlock.hpp>
#include <benchmark/benchmark.h>
#include <cstddef>
#include <string>

namespace apl::test {

template<typename T>
struct seqlock: benchmark::Fixture {
  apl::seqlock<T> seqlock{};
};

BENCHMARK_TEMPLATE_F(seqlock, store_lvalue, detail::pod_type)(benchmark::State& state) {
  const auto value = detail::pod_type();
  for (auto _: state) {
    seqlock.store(value);
  }
}

BENCHMARK_TEMPLATE_F(seqlock, store_rvalue, detail::pod_type)(benchmark::State& state) {
  for (auto _: state) {
    seqlock.store(detail::pod_type());
  }
}

BENCHMARK_TEMPLATE_F(seqlock, read, detail::pod_type)(benchmark::State& state) {
  auto loaded_value = detail::pod_type{};
  for (auto _: state) {
    benchmark::DoNotOptimize(seqlock.load(loaded_value));
  }
}

}
