add_library_unity(graindb_common_operators OBJECT cast_operators.cpp)
set(ALL_OBJECT_FILES ${ALL_OBJECT_FILES}
                     $<TARGET_OBJECTS:graindb_common_operators> PARENT_SCOPE)
