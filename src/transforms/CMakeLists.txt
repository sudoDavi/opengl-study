cmake_minimum_required(VERSION 3.10)

project(OpenGLStudy-Triangles)

set(
	TUTORIALS
	"camera"
	"coords"
	"transform"
)

foreach(TUTORIAL ${TUTORIALS})
	add_subdirectory(${TUTORIAL})
endforeach()
