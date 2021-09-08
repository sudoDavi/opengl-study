cmake_minimum_required(VERSION 3.10)

project(OpenGLStudy)

# Add GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS ON)
add_subdirectory(lib/glfw)

# Add Assimp
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON)
add_subdirectory(lib/assimp)

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

