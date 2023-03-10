find_package(Java 1.8)
find_package(JNI)

cmake_minimum_required(VERSION 3.11.0)


if(NOT JNI_FOUND OR NOT Java_FOUND)
  message(FATAL_ERROR No
                      compatible
                      Java/JNI
                      found)
endif()

include(UseJava)
project(GrainDBJDummy NONE)

file(GLOB JAVA_SRC_FILES src/main/java/nl/cwi/da/graindb/*.java)
file(GLOB JAVA_TEST_FILES src/test/java/nl/cwi/da/graindb/test/*.java)

add_jar(graindb_jdbc
        ${JAVA_SRC_FILES}
        ${JAVA_TEST_FILES}
        GENERATE_NATIVE_HEADERS
        graindb-native)

add_library(graindb_java SHARED src/jni/graindb_java.cpp)
target_compile_options(graindb_java PRIVATE -fexceptions)
target_link_libraries(graindb_java graindb-native graindb_static)
set_target_properties(graindb_java PROPERTIES SUFFIX ".so")
set_target_properties(graindb_java PROPERTIES PREFIX "lib")

add_custom_command(OUTPUT dummy_jdbc_target
                   DEPENDS graindb_java graindb_jdbc
                   COMMAND ${Java_JAR_EXECUTABLE}
                           uf
                           graindb_jdbc.jar
                           -C
                           $<TARGET_FILE_DIR:graindb_java>
                           $<TARGET_FILE_NAME:graindb_java>)

add_custom_target(jdbc ALL DEPENDS dummy_jdbc_target)
