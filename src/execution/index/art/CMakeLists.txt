add_library_unity(
  graindb_art_index_execution
  OBJECT
  art_key.cpp
  leaf.cpp
  node.cpp
  node4.cpp
  node16.cpp
  node48.cpp
  node256.cpp
  art.cpp)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:graindb_art_index_execution>
    PARENT_SCOPE)
