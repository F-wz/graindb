add_library_unity(graindb_operator_helper
                  OBJECT
                  physical_execute.cpp
                  physical_limit.cpp
                  physical_pragma.cpp
                  physical_prepare.cpp
                  physical_transaction.cpp
                  physical_vacuum.cpp)
set(ALL_OBJECT_FILES ${ALL_OBJECT_FILES}
                     $<TARGET_OBJECTS:graindb_operator_helper> PARENT_SCOPE)
