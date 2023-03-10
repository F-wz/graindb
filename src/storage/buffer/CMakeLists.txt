add_library_unity(graindb_storage_buffer
                  OBJECT
                  buffer_handle.cpp
                  buffer_list.cpp
                  managed_buffer.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:graindb_storage_buffer>
    PARENT_SCOPE)
