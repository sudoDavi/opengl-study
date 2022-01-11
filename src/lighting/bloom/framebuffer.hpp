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
		DepthStencil,
	};

private:
	std::uint32_t m_colorBuffer{};
	std::uint32_t m_colorBuffer1{};
	std::uint32_t m_depthBuffer;
	std::uint32_t m_stencilBuffer;
	bool m_combinedBuffers{ false };

private:
	void CreateBuffer(BufferType type);
	void CreateBuffer(BufferType type, uint32_t* colorBuffer_p);
	void AttachBuffer(BufferType type);
	void AttachBuffer(BufferType type, uint32_t colorBuffer, GLenum colorAttachment);

public:
	std::uint32_t Id{};
	AttachTyp AttachmentType{};

public:
	Framebuffer(bool createDepthBuffer = false, bool createStencilBuffer = false, AttachTyp attachmentType = AttachTyp::Texture);
	
	void Bind(GLenum target = GL_FRAMEBUFFER) const;

	const std::uint32_t GetColorBuffer() const;
};

#endif
