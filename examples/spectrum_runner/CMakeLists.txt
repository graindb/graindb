cmake_minimum_required(VERSION 2.8.12)
project(spectrum_runner)

set(CMAKE_CXX_STANDARD 11)

include_directories(../../third_party/dbgen/include)
include_directories(../../third_party/ldbc/include)
include_directories(../../third_party/imdb/include)
link_directories(../../build/release/src)

add_executable(spectrum_runner spectrum_runner.cpp)
target_link_libraries(spectrum_runner duckdb dbgen ldbc imdb nlohmann_json)
