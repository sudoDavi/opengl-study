#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <glad/glad.h>

#include <cstdint>
#include <vector>
#include <iostream>



class Framebuffer {
public:
	enum class AttachTyp {
		Texture,
		RenderBuffer
	};

private:
	enum class BufferType {
		Color,
		Depth,
		Stencil,
		DepthStencil
	};

private:
	std::uint32_t m_colorBuffer{};
	std::uint32_t m_depthBuffer;
	std::uint32_t m_stencilBuffer;
	bool m_combinedBuffers{ false };

private:
	void CreateBuffer(BufferType type, std::uint32_t nmrSamples = 1);
	void AttachBuffer(BufferType type, std::uint32_t nmrSamples = 1);

public:
	std::uint32_t Id{};
	AttachTyp AttachmentType{};
	bool IsMultisample{ false };
	std::uint32_t NmrSamples{};

public:
	Framebuffer(bool createDepthBuffer = false, bool createStencilBuffer = false, AttachTyp attachmentType = AttachTyp::Texture,
			std::uint32_t nmrSamples = 1);
	
	void Bind(GLenum target = GL_FRAMEBUFFER) const;
	const std::uint32_t GetColorBuffer() const;
};

#endif
