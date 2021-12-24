include_directories(../../third_party/ldbc/include)

add_library(duckdb_benchmark_ldbc OBJECT ldbc_light_optimized.cpp ldbc_light_default.cpp
        ldbc_micro_08.cpp ldbc_micro_09.cpp)
set(BENCHMARK_OBJECT_FILES ${BENCHMARK_OBJECT_FILES}
    $<TARGET_OBJECTS:duckdb_benchmark_ldbc> PARENT_SCOPE)
