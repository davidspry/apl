cmake_minimum_required(VERSION 3.14)
project(apl_events_test VERSION 0.1.0 LANGUAGES CXX)
set(TARGET ${PROJECT_NAME})

#===========================================
find_package(GTest REQUIRED)

#===========================================
cmake_path(SET APL_INCLUDE NORMALIZE "../../")

#===========================================
add_executable(${TARGET} test_apl_events.cpp)

set_target_properties(${TARGET}
  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON)

target_include_directories(${TARGET}
  PRIVATE
    ${APL_INCLUDE})

target_link_libraries(${TARGET}
  PRIVATE
    GTest::GTest)
