cmake_minimum_required(VERSION 3.10)

project(OpenGLStudy-Triangles)

set(
	TUTORIALS
	"window"
	"triangle"
	"rectangle"
	"triangle-exercise"
	"uniforms"
	"attribs-delight"
)

foreach(TUTORIAL ${TUTORIALS})
	add_subdirectory(${TUTORIAL})
endforeach()
