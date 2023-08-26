//
// Created by David Spry on 20/1/2023.
//

#pragma once

#include "../test/test_types.hpp"

#include <apl_threading/include/cexlock.hpp>
#include <benchmark/benchmark.h>

namespace apl::test {

template<typename T>
struct cexlock: benchmark::Fixture {
  apl::cexlock<T> cexlock{};
};

BENCHMARK_TEMPLATE_F(cexlock, copy_read, detail::pod_type)(benchmark::State& state) {
  for (auto _: state) {
    benchmark::DoNotOptimize(
      cexlock.copy_read()
    );
  }
}

BENCHMARK_TEMPLATE_F(cexlock, lock_read, detail::pod_type)(benchmark::State& state) {
  for (auto _: state) {
    benchmark::DoNotOptimize(
      cexlock.lock_read().operator*()
    );
  }
}

BENCHMARK_TEMPLATE_F(cexlock, write, detail::pod_type)(benchmark::State& state) {
  const auto value = detail::pod_type{1, 2, "3"};
  for (auto _: state) {
    cexlock.write(value);
  }
}

BENCHMARK_TEMPLATE_F(cexlock, copy_read_then_write, detail::pod_type)(benchmark::State& state) {
  for (auto _: state) {
    cexlock.write(
      cexlock.copy_read()
    );
  }
}

}
