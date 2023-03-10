add_library_unity(graindb_parsed_data OBJECT alter_table_info.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:graindb_parsed_data>
    PARENT_SCOPE)
