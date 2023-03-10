add_library_unity(graindb_func_sqlite
                  OBJECT
                  pragma_collations.cpp
                  pragma_table_info.cpp
                  sqlite_master.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:graindb_func_sqlite>
    PARENT_SCOPE)
