add_library_unity(graindb_func_seq OBJECT nextval.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:graindb_func_seq>
    PARENT_SCOPE)
