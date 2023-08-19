//
// Created by David Spry on 20/1/2023.
//

#pragma once

#include <apl_threading/include/spinlock.hpp>
#include <benchmark/benchmark.h>
#include <thread>

namespace apl::test {

struct spinlock: public benchmark::Fixture {
  apl::spinlock spinlock{};
  int data{};

  void SetUp(benchmark::State&) override {
    data = 128;
  }
};

BENCHMARK_DEFINE_F(spinlock, lock_process_unlock)(benchmark::State& state) {
  for (auto _: state) {
    spinlock.lock();
    data <<= 3;
    data |= 63;
    data >>= 4;
    data |= 63;
    data <<= 2;
    data &= 255;
    spinlock.unlock();
  }
}

BENCHMARK_REGISTER_F(spinlock, lock_process_unlock)
->ThreadRange(1, std::thread::hardware_concurrency());

}
