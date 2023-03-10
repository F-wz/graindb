add_library_unity(graindb_aggr_nested
                  OBJECT
                 list.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:graindb_aggr_nested>
    PARENT_SCOPE)
