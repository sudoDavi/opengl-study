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
	"depth-testing"
	"stencil-testing"
	"blending"
	"culling"
	"framebuffers"
	"cubemaps"
	"uniform-buffer"
	"geometry-shaders"
	"instancing"
	"anti-aliasing"
	"deferred-shading"
)

foreach(TUTORIAL ${TUTORIALS})
	add_subdirectory(${TUTORIAL})
endforeach()
