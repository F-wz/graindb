add_library_unity(graindb_operator_filter OBJECT physical_filter.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:graindb_operator_filter>
    PARENT_SCOPE)
