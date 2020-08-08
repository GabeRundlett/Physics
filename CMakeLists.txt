cmake_minimum_required(VERSION 3.16)

project(Physics)
include(project.cmake)

conan_cmake_run(
    BASIC_SETUP
    CMAKE_TARGETS
    REQUIRES
    glew/2.1.0
    glfw/3.3.2
    stb/20200203
    BUILD
)

add_subdirectory(Physics)
add_subdirectory(Master)
