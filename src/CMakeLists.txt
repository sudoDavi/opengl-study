cmake_minimum_required(VERSION 3.10)

project(OpenGLStudy-Tutorials)

set(
	TUTORIALS
	"textures"
	"shader"
	"transforms"
	"triangles"
	"lighting"
	"model-loading"
)

foreach(TUTORIAL ${TUTORIALS})
	add_subdirectory(${TUTORIAL})
endforeach()
