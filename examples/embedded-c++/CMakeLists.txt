cmake_minimum_required(VERSION 2.8.7)
project(example-c++)

set(CMAKE_CXX_STANDARD 11)

include_directories(../../src/include)
include_directories(../../third_party/dsdgen/include)
include_directories(../../third_party/dbgen/include)
include_directories(../../third_party/ldbc/include)
include_directories(../../third_party/imdb/include)
link_directories(../../build/release/src)

add_executable(example-c++ main.cpp)
target_link_libraries(example-c++ duckdb dbgen dsdgen ldbc imdb nlohmann_json)
