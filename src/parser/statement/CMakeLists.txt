add_library_unity(graindb_statement OBJECT select_statement.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:graindb_statement>
    PARENT_SCOPE)
