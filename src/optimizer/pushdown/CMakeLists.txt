add_library_unity(graindb_optimizer_pushdown
                  OBJECT
                  pushdown_aggregate.cpp
                  pushdown_cross_product.cpp
                  pushdown_filter.cpp
                  pushdown_get.cpp
                  pushdown_inner_join.cpp
                  pushdown_left_join.cpp
                  pushdown_mark_join.cpp
                  pushdown_projection.cpp
                  pushdown_set_operation.cpp
                  pushdown_single_join.cpp)
set(ALL_OBJECT_FILES ${ALL_OBJECT_FILES}
                     $<TARGET_OBJECTS:graindb_optimizer_pushdown> PARENT_SCOPE)
