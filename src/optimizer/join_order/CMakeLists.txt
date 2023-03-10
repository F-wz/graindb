add_library_unity(graindb_optimizer_join_order
                  OBJECT
                  query_graph.cpp
                  relation.cpp)
set(ALL_OBJECT_FILES ${ALL_OBJECT_FILES}
                     $<TARGET_OBJECTS:graindb_optimizer_join_order> PARENT_SCOPE)
