add_library_unity(graindb_nested_loop_join OBJECT nested_loop_join_inner.cpp
                  nested_loop_join_mark.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:graindb_nested_loop_join>
    PARENT_SCOPE)
