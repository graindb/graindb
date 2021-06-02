add_library_unity(test_sql_job
                  OBJECT
                  #test_job.cpp
                  test_job_small.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:test_sql_job>
    PARENT_SCOPE)
