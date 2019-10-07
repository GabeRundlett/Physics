file(GLOB_RECURSE PHYSICS_SOURCES "src/*.cpp" "src/*.hpp")
add_library(physics_lib STATIC ${PHYSICS_SOURCES})
