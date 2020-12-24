cmake_minimum_required(VERSION 3.10)

project(OpenGLStudy)

# Add GLFW
find_package(glfw3)

# Add GLAD
add_subdirectory(lib/glad)

set(
	TUTORIALS
	"triangles"
	"shader"
	"textures"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

foreach(TUTORIAL ${TUTORIALS})
	add_subdirectory(${TUTORIAL})
endforeach()
