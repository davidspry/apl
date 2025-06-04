cmake_minimum_required(VERSION 3.12)

project(
  APL_SYNC
  DESCRIPTION "Concurrency utilities"
  VERSION 0.1.0
  LANGUAGES CXX)

string(TOLOWER ${PROJECT_NAME} TARGET)
string(REPLACE "_" "::" TARGET_ALIAS ${TARGET})

add_library(${TARGET} INTERFACE)
add_library(${TARGET_ALIAS} ALIAS ${TARGET})

target_compile_features(${TARGET}
  INTERFACE
    cxx_std_20)

target_include_directories(${TARGET}
  INTERFACE
    include)
