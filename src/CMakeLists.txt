cmake_minimum_required(VERSION 3.10)

project(OpenGLStudy-Tutorials)

set(
	TUTORIALS
	"textures"
	"shader"
	"transforms"
	"triangles"
)

foreach(TUTORIAL ${TUTORIALS})
	add_subdirectory(${TUTORIAL})
endforeach()
