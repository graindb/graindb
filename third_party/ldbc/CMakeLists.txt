cmake_minimum_required(VERSION 2.8.12)

project(ldbc CXX)
cmake_policy(SET CMP0063 NEW)
include_directories(include)

set(CMAKE_BUILD_TYPE "Release")
add_library(ldbc STATIC ldbc.cpp ${ALL_OBJECT_FILES})

disable_target_warnings(ldbc)
