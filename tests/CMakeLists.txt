cmake_minimum_required(VERSION 3.14)

project(apl_tests LANGUAGES CXX)
set(tests_target apl_tests)
set(bench_target apl_bench)

find_package(fmt REQUIRED)
find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

function(initialise_target target_name)
    set_target_properties(${target_name}
            PROPERTIES
                CXX_STANDARD 23
                CXX_STANDARD_REQUIRED ON)

    target_compile_options(${target_name}
            PRIVATE
                -fexperimental-library)

    target_include_directories(${target_name}
            PRIVATE
                ../include)
endfunction()

add_executable(${tests_target} unit_tests.cpp)
add_executable(${bench_target} bench.cpp)

initialise_target(${tests_target})
initialise_target(${bench_target})

target_link_libraries(${tests_target} PRIVATE fmt::fmt)
target_link_libraries(${tests_target} PRIVATE GTest::GTest)
target_link_libraries(${bench_target} PRIVATE benchmark::benchmark)
