file(GLOB_RECURSE MainSources src/*.cpp)
add_executable(Master ${MainSources})
target_link_libraries(Master 
    Physics
    CONAN_PKG::glew
)
