file(GLOB_RECURSE MainSources src/*.cpp)
add_executable(Master ${MainSources})
target_link_libraries(Master 
    # Physics
    CONAN_PKG::glew
    CONAN_PKG::glfw
    CONAN_PKG::stb
    project_options
    project_warnings
)
target_compile_features(Master PRIVATE cxx_std_20)
