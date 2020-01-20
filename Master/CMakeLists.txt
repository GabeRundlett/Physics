file(GLOB_RECURSE MainSources "src/*.cpp")
add_executable(Master ${MainSources})
target_link_libraries(Master Debug Physics glfw glad)
target_include_directories(Master PRIVATE ${PhysicsIncludePath} ${GlfwIncludePath} ${GladIncludePath})
