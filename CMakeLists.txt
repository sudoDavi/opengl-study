cmake_minimum_required(VERSION 3.10)

project(OpenGLStudy)

# Add GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS ON)
add_subdirectory(lib/glfw)

# Add GLAD
add_subdirectory(lib/glad)

# Add GLM
add_subdirectory(lib/glm)

# Add all header files inside lib/
include_directories(lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Add the src folder
add_subdirectory(src)

