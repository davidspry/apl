cmake_minimum_required(VERSION 3.12)

project(APL_APP CXX)

string(TOLOWER ${PROJECT_NAME} TARGET)

add_executable(${TARGET} main.cpp)

add_custom_target(run_${TARGET}
  COMMAND ${TARGET}
  DEPENDS ${TARGET}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(${TARGET}
  PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON)

target_link_libraries(${TARGET}
  PRIVATE
    apl::apl)
