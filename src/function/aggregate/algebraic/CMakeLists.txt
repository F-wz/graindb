add_library_unity(graindb_aggr_algebraic OBJECT avg.cpp covar.cpp stddev.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:graindb_aggr_algebraic>
    PARENT_SCOPE)
