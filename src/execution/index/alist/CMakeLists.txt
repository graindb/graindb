add_library_unity(duckdb_alist_index_execution OBJECT alist.cpp)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:duckdb_alist_index_execution>
    PARENT_SCOPE)
