include_directories(../third_party/catch)
include_directories(../third_party/sqlite/include)
include_directories(../test/include)
include_directories(../tools/dbtransfer/include)
include_directories(include)

add_subdirectory(dbwrapper)
add_subdirectory(tpch)
add_subdirectory(imdb)
add_subdirectory(ldbc)

add_executable(benchmark_runner benchmark_runner.cpp ${BENCHMARK_OBJECT_FILES})
target_link_libraries(benchmark_runner
                      duckdb
                      dbgen
                      imdb
                      ldbc
                      test_helpers
                      sqlite_transfer)
