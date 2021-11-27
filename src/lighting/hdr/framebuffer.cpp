#include "framebuffer.hpp"

Framebuffer::Framebuffer(bool createDepthBuffer, bool createStencilBuffer, Framebuffer::AttachTyp attachmentType)
	: AttachmentType{ attachmentType }
{
	glGenFramebuffers(1, &Id);
	glBindFramebuffer(GL_FRAMEBUFFER, Id);

	CreateBuffer(BufferType::Color);
	AttachBuffer(BufferType::Color);


	if (createDepthBuffer && createStencilBuffer) {
		CreateBuffer(BufferType::DepthStencil);
		AttachBuffer(BufferType::DepthStencil);
	}
	else if (createDepthBuffer) {
		CreateBuffer(BufferType::Depth);
		AttachBuffer(BufferType::Depth);
	}
	else if (createStencilBuffer) {
		CreateBuffer(BufferType::Stencil);
		AttachBuffer(BufferType::Stencil);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER::NOT_COMPLETE\n";

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Framebuffer::AttachBuffer(BufferType type) {
	if (AttachmentType == Framebuffer::AttachTyp::Texture) {
		switch (type) {
			case BufferType::Color :
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer, 0);
				break;
			case BufferType::Depth :
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBuffer, 0);
				break;
			case BufferType::Stencil :
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_stencilBuffer, 0);
				break;
			case BufferType::DepthStencil :
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthBuffer, 0);
				break;
			default:
				std::cout << "ERROR::FRAMEBUFFER::INVALID_BUFFERTYPE\n";
		}
	}
	else if (AttachmentType == Framebuffer::AttachTyp::RenderBuffer) {
		switch (type) {
			case BufferType::Color :
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer, 0);
				break;
			case BufferType::Depth :
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
				break;
			case BufferType::Stencil :
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_stencilBuffer);
				break;
			case BufferType::DepthStencil :
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
				break;
			default:
				std::cout << "ERROR::FRAMEBUFFER::INVALID_BUFFERTYPE\n";
		}
	}
}



void Framebuffer::CreateBuffer(Framebuffer::BufferType type) {
	if ( AttachmentType == Framebuffer::AttachTyp::Texture) {
		switch (type) {
			case BufferType::Color :
				glGenTextures(1, &m_colorBuffer);
				glBindTexture(GL_TEXTURE_2D, m_colorBuffer);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			case BufferType::Stencil :
				glGenTextures(1, &m_stencilBuffer);
				glBindTexture(GL_TEXTURE_2D, m_stencilBuffer);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, 800, 600, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, NULL);
				break;
			case BufferType::Depth :
				glGenTextures(1, &m_depthBuffer);
				glBindTexture(GL_TEXTURE_2D, m_depthBuffer);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 800, 600, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
				break;
			case BufferType::DepthStencil :
				m_combinedBuffers = true;

				glGenTextures(1, &m_depthBuffer);
				glBindTexture(GL_TEXTURE_2D, m_depthBuffer);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 800, 600, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
				break;
			default:
				std::cout << "ERROR::FRAMEBUFFER::INVALID_BUFFERTYPE\n";
		}
	}
	else if (AttachmentType == Framebuffer::AttachTyp::RenderBuffer) {
		switch (type) {
			case BufferType::Color :
				glGenTextures(1, &m_colorBuffer);
				glBindTexture(GL_TEXTURE_2D, m_colorBuffer);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			case BufferType::Stencil :
				glGenRenderbuffers(1, &m_stencilBuffer);
				glBindRenderbuffer(GL_RENDERBUFFER, m_stencilBuffer);

				glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX, 800, 600);
				break;
			case BufferType::Depth :
				glGenRenderbuffers(1, &m_depthBuffer);
				glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);

				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 600);
				break;
			case BufferType::DepthStencil :
				m_combinedBuffers = true;

				glGenRenderbuffers(1, &m_depthBuffer);
				glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);

				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
				break;
			default:
				std::cout << "ERROR::FRAMEBUFFER::INVALID_BUFFERTYPE\n";
		}
	}
}





void Framebuffer::Bind(GLenum target) const {
	glBindFramebuffer(target, Id);
}

const std::uint32_t Framebuffer::GetColorBuffer() const {
	return m_colorBuffer;
}

