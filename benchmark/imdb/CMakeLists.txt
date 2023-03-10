include_directories(../../third_party/imdb/include)

add_library(graindb_benchmark_imdb OBJECT imdb_optimized.cpp)
set(BENCHMARK_OBJECT_FILES ${BENCHMARK_OBJECT_FILES}
    $<TARGET_OBJECTS:graindb_benchmark_imdb> PARENT_SCOPE)
