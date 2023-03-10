set(GRAINDB_TEST_HELPERS_UNITS expression_helper.cpp test_helpers.cpp)

add_library(test_helpers STATIC ${GRAINDB_TEST_HELPERS_UNITS})

if(NOT WIN32)
  target_link_libraries(test_helpers graindb)
endif()
