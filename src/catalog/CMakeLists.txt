add_subdirectory(catalog_entry)

add_library_unity(graindb_catalog
                  OBJECT
                  catalog_entry.cpp
                  catalog.cpp
                  catalog_set.cpp
                  dependency_manager.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:graindb_catalog>
    PARENT_SCOPE)
