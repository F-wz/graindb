add_library_unity(graindb_storage_checkpoint
                  OBJECT
                  table_data_writer.cpp
                  table_data_reader.cpp)
set(ALL_OBJECT_FILES ${ALL_OBJECT_FILES}
                     $<TARGET_OBJECTS:graindb_storage_checkpoint> PARENT_SCOPE)
