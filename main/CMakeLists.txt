include_directories("${DEBUG_INCLUDE_PATH}")
include_directories("${PHYSICS_INCLUDE_PATH}")

file(GLOB_RECURSE MAIN_SOURCES "src/*.cpp" "src/*.hpp")
add_executable(main ${MAIN_SOURCES})
target_link_libraries(main general debug_lib physics_lib)
