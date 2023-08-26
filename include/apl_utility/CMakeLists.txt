cmake_minimum_required(VERSION 3.12)
project(apl_utility VERSION 0.1.0 LANGUAGES CXX)
set(TARGET ${PROJECT_NAME})

#===========================================
option(APL_BUILD_TESTS "Build unit-tests and benchmarking targets" OFF)

if (APL_BUILD_TESTS)
  add_subdirectory(bench)
  add_subdirectory(test)
endif ()

#===========================================
add_library(${TARGET} INTERFACE)

set_target_properties(${TARGET}
  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON)

target_include_directories(${TARGET}
  INTERFACE
    include)
