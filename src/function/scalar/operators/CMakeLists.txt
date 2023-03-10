add_library_unity(graindb_func_ops OBJECT arithmetic.cpp bitwise.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:graindb_func_ops>
    PARENT_SCOPE)
