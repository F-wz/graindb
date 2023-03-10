add_library_unity(graindb_func_math
                  OBJECT
                  numeric.cpp
                  random.cpp
                  setseed.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:graindb_func_math>
    PARENT_SCOPE)
