#ifndef DEPTH_FRAMEBUFFER_HPP
#define DEPTH_FRAMEBUFFER_HPP

#include <glad/glad.h>

#include <cstdint>

class DepthFB {
public:
	std::uint32_t TexId;
	std::uint32_t FBO;
	std::uint32_t Height, Width;
public:
	DepthFB(std::uint32_t width, std::uint32_t height);
	Bind();
};
#endif
