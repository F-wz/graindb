add_subdirectory(sqlite)
add_library_unity(graindb_func_table
                  OBJECT
                  read_csv.cpp
                  sqlite_functions.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:graindb_func_table>
    PARENT_SCOPE)
