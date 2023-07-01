cmake_minimum_required(VERSION 3.14)
project(apl_bench VERSION 0.1.0 LANGUAGES CXX)
set(TARGET ${PROJECT_NAME})

#===========================================
find_package(benchmark REQUIRED)

#===========================================
add_executable(${TARGET} bench.cpp)

set_target_properties(${TARGET}
        PROPERTIES
            CXX_STANDARD 23
            CXX_STANDARD_REQUIRED ON)

target_compile_options(${TARGET}
        PRIVATE
            -fexperimental-library)

target_include_directories(${TARGET}
        PRIVATE
            ../include)

target_link_libraries(${TARGET}
        PRIVATE
            benchmark::benchmark)
