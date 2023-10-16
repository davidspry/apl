#include <apl_events/test/test_apl_events.hpp>
#include <apl_functional/test/test_apl_functional.hpp>
#include <apl_threading/test/test_apl_threading.hpp>
#include <apl_utility/test/test_apl_utility.hpp>
#include <gtest/gtest.h>

auto main(int argc, char** argv) -> int {
  using namespace testing;
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
