cmake_minimum_required(VERSION 2.8.12)

project(pubmed CXX)
cmake_policy(SET CMP0063 NEW)
include_directories(include)

set(CMAKE_BUILD_TYPE "Release")
add_library(pubmed STATIC pubmed.cpp ${ALL_OBJECT_FILES})

disable_target_warnings(pubmed)
