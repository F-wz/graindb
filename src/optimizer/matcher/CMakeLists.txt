add_library_unity(graindb_optimizer_matcher OBJECT expression_matcher.cpp)
set(ALL_OBJECT_FILES ${ALL_OBJECT_FILES}
                     $<TARGET_OBJECTS:graindb_optimizer_matcher> PARENT_SCOPE)
