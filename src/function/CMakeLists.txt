add_subdirectory(aggregate)
add_subdirectory(scalar)
add_subdirectory(table)

add_library_unity(graindb_function
                  OBJECT
                  cast_rules.cpp
                  function.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:graindb_function>
    PARENT_SCOPE)
