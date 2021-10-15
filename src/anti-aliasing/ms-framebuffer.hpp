#ifndef MS_FRAMEBUFFER_HPP
#define MS_FRAMEBUFFER_HPP

#include <glad/glad.h>

#include <cstdint>
#include <vector>
#include <iostream>

class MSFramebuffer {
private:
	std::uint32_t m_colorBuffer{};
	std::uint32_t m_depthStencilBuffer{};

public:
	std::uint32_t Id{};

public:
	MSFramebuffer(std::uint32_t nmrSamples);
	
	void Bind(GLenum target = GL_FRAMEBUFFER) const;

	const std::uint32_t GetColorBuffer() const;
};

#endif
