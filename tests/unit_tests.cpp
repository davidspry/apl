//
// Created by David Spry on 5/10/2022.
//

#include "array_tests.hpp"
#include "cexlock_tests.hpp"
#include "double_buffer_tests.hpp"
#include "mod_tests.hpp"
#include "mwmr_queue_test.hpp"
#include "notify_tests.hpp"
#include "random_tests.hpp"
#include "range_tests.hpp"
#include "seqlock_tests.hpp"
#include "shared_tests.hpp"
#include "swsr_queue_test.hpp"
#include "thread_group_tests.hpp"
#include <gtest/gtest.h>

auto main(int argc, char** argv) -> int {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
