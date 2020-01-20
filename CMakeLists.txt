cmake_minimum_required(VERSION 3.6)

project(Physics)

set(PhysicsIncludePath "${PROJECT_SOURCE_DIR}/Physics/src")
set(GlfwIncludePath "${PROJECT_SOURCE_DIR}/glfw/include")
set(GladIncludePath "${PROJECT_SOURCE_DIR}/glad/include")

add_subdirectory(glad)
add_subdirectory(glfw)

add_subdirectory(Physics)
add_subdirectory(Master)
# add_subdirectory(Test)
