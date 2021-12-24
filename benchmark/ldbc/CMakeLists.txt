include_directories(../../third_party/ldbc/include)

add_library(duckdb_benchmark_ldbc OBJECT ldbc_light_optimized.cpp ldbc_light_views.cpp
        ldbc_micro_p.cpp ldbc_micro_k.cpp)
set(BENCHMARK_OBJECT_FILES ${BENCHMARK_OBJECT_FILES}
    $<TARGET_OBJECTS:duckdb_benchmark_ldbc> PARENT_SCOPE)
