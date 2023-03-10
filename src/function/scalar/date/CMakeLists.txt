add_library_unity(graindb_func_date OBJECT age.cpp current.cpp date_trunc.cpp epoch.cpp
                  date_part.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:graindb_func_date>
    PARENT_SCOPE)
