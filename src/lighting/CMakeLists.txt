cmake_minimum_required(VERSION 3.10)

project(OpenGLStudy-Lighting)

set(
	TUTORIALS
	"basic-light"
	"materials"
	"light-maps"
	"light-casters"
	"multiple-lights"
	"gamma-correction"
	"shadows"
	"normal-maps"
	"parallax-mapping"
	"hdr"
	"bloom"
)

foreach(TUTORIAL ${TUTORIALS})
	add_subdirectory(${TUTORIAL})
endforeach()
