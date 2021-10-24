cmake_minimum_required(VERSION 3.10)

project(OpenGLStudy-Lighting-Shadows)

set(
	TUTORIALS
	"shadow-mapping"
)

foreach(TUTORIAL ${TUTORIALS})
	add_subdirectory(${TUTORIAL})
endforeach()
