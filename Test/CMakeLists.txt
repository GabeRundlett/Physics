file(GLOB_RECURSE TestSources "src/*.cpp")
add_executable(Test ${TestSources})
target_link_libraries(Test Debug Physics glfw glad)
target_include_directories(Test PRIVATE ${PhysicsIncludePath} ${GlfwIncludePath} ${GladIncludePath})
