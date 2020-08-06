file(GLOB_RECURSE PhysicsSources src/*.cpp)
add_library(Physics ${PhysicsSources})
target_include_directories(Physics PUBLIC src)
