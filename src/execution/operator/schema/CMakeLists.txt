add_library_unity(
  graindb_operator_schema
  OBJECT
  physical_alter.cpp
  physical_create_index.cpp
  physical_create_rai.cpp
  physical_create_schema.cpp
  physical_create_sequence.cpp
  physical_create_table.cpp
  physical_create_view.cpp
  physical_drop.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:graindb_operator_schema>
    PARENT_SCOPE)
