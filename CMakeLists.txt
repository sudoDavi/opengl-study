cmake_minimum_required(VERSION 3.10)

project(OpenGLStudy)

# Add GLFW
find_package(glfw3)

# Add GLAD
add_subdirectory(glad)

set(
	TUTORIALS
	"window"
	"triangle"
	"rectangle"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

foreach(TUTORIAL ${TUTORIALS})
	add_subdirectory(${TUTORIAL})
endforeach()
