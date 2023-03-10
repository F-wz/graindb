include_directories(../../third_party/sqlite/include)
add_library(graindb_benchmark_dbwrappers OBJECT sqlite_benchmark.cpp)
set(BENCHMARK_OBJECT_FILES
    ${BENCHMARK_OBJECT_FILES} $<TARGET_OBJECTS:graindb_benchmark_dbwrappers>
    PARENT_SCOPE)
