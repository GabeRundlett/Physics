file(GLOB_RECURSE PhysicsSources "src/*.cpp")
add_library(Physics STATIC ${PhysicsSources})
