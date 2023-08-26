//! apl
//! David Spry
//! Created on 26/8/2023

#include <gtest/gtest.h>

#include "test_apl_threading.hpp"

auto main(int argc, char** argv) -> int {
  using namespace testing;
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
