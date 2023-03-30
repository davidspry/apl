cmake_minimum_required(VERSION 3.14)

project(apl VERSION 0.1.0 LANGUAGES CXX)
set(target ${PROJECT_NAME})

add_library(${target} INTERFACE)

function(initialise_target target_name)
    set_target_properties(${target_name}
            PROPERTIES
                CXX_STANDARD 23
                CXX_STANDARD_REQUIRED ON)

    target_include_directories(${target_name}
            INTERFACE
                include)
endfunction()

initialise_target(${target})
