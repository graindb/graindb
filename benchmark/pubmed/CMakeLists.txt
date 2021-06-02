include_directories(../../third_party/pubmed/include)

add_library(duckdb_benchmark_pubmed OBJECT pubmed_small.cpp pubmed_large.cpp)
set(BENCHMARK_OBJECT_FILES ${BENCHMARK_OBJECT_FILES}
                           $<TARGET_OBJECTS:duckdb_benchmark_pubmed> PARENT_SCOPE)
