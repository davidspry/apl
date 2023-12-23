//
// Created by David Spry on 20/1/2023.
//

#pragma once

#include <apl_utility/include/random.hpp>
#include <benchmark/benchmark.h>

namespace apl::test {

struct random: benchmark::Fixture {
  apl::random random{};
};

BENCHMARK_F(random, xoroshiro128p)(benchmark::State& state) {
  for (auto _: state) {
    benchmark::DoNotOptimize(
      random()
    );
  }
}

BENCHMARK_F(random, integral_u32)(benchmark::State& state) {
  for (auto _: state) {
    benchmark::DoNotOptimize(
      random.operator()<std::uint32_t>()
    );
  }
}

BENCHMARK_F(random, integral_bool)(benchmark::State& state) {
  for (auto _: state) {
    benchmark::DoNotOptimize(
      random.operator()<bool>()
    );
  }
}

BENCHMARK_F(random, uniform_f32)(benchmark::State& state) {
  for (auto _: state) {
    benchmark::DoNotOptimize(
      random.uniform<float>()
    );
  }
}

BENCHMARK_F(random, uniform_f64)(benchmark::State& state) {
  for (auto _: state) {
    benchmark::DoNotOptimize(
      random.uniform<double>()
    );
  }
}

BENCHMARK_F(random, normal_f32)(benchmark::State& state) {
  for (auto _: state) {
    benchmark::DoNotOptimize(
      random.normal<float>()
    );
  }
}

BENCHMARK_F(random, normal_f64)(benchmark::State& state) {
  for (auto _: state) {
    benchmark::DoNotOptimize(
      random.normal<double>()
    );
  }
}

}
