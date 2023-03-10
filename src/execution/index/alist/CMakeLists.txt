add_library_unity(graindb_alist_index_execution OBJECT alist.cpp)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:graindb_alist_index_execution>
    PARENT_SCOPE)
