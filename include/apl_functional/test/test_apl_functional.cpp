//! apl
//! David Spry
//! Created on 16/10/2023

#include <gtest/gtest.h>

#include "test_apl_functional.hpp"

auto main(int argc, char** argv) -> int {
  using namespace testing;
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
