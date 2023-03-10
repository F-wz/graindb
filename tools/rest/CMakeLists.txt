include_directories(.)
include_directories(../../third_party/dbgen/include)

add_definitions(-DGRAINDB_SOURCE_ID="\""${GIT_COMMIT_HASH}"\"")

add_executable(graindb_rest_server server.cpp)

if(${BUILD_SUN})
  set(LINK_EXTRA -lsocket)
endif()

target_link_libraries(graindb_rest_server graindb_static Threads::Threads ${LINK_EXTRA})

add_executable(graindb_dbgen dbgen.cpp)
target_link_libraries(graindb_dbgen dbgen graindb_static)
