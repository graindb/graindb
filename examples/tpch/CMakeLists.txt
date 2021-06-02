cmake_minimum_required(VERSION 2.8.7)
project(tpch)

set(CMAKE_CXX_STANDARD 11)

include_directories(../../src/include)
link_directories(../../build/release/src)

add_executable(tpch main.cpp)
target_link_libraries(tpch duckdb)