cmake_minimum_required(VERSION 3.6)

# project name
project(physics)

# includes
set(DEBUG_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/debug/src")
set(PHYSICS_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/physics/src")
set(GLFW_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/glfw/include")
set(GLAD_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/glad/include")

# binaries
add_subdirectory(debug)
add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(physics)
add_subdirectory(main)
add_subdirectory(test)

# move ALL_BUILD, ZERO_CHECK, INSTALL, 
# PACKAGE and RUN_TESTS into their own folder
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# set the default startup executable to main
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)
