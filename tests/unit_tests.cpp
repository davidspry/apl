//
// Created by David Spry on 5/10/2022.
//

#include "array_tests.hpp"
#include "thread_group_tests.hpp"

#include <gtest/gtest.h>

auto main(int argc, char** argv) -> int {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
