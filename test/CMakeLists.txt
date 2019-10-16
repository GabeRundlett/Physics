include_directories("${DEBUG_INCLUDE_PATH}")
include_directories("${PHYSICS_INCLUDE_PATH}")
include_directories("${GLFW_INCLUDE_PATH}")
include_directories("${GLAD_INCLUDE_PATH}")

file(GLOB_RECURSE TEST_SOURCES "src/*.cpp" "src/*.hpp")
add_executable(test ${TEST_SOURCES})
target_link_libraries(test general debug_lib physics_lib glfw glad)
