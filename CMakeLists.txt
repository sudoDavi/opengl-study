cmake_minimum_required(VERSION 3.10)

project(OpenGLStudy)

# Add GLFW
find_package(glfw3)

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

