#include "ms-framebuffer.hpp"

MSFramebuffer::MSFramebuffer(std::uint32_t nmrSamples)
{
	glGenFramebuffers(1, &Id);
	glBindFramebuffer(GL_FRAMEBUFFER, Id);

	glGenTextures(1, &m_colorBuffer);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_colorBuffer);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, nmrSamples, GL_RGB, 800, 600, GL_TRUE);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_colorBuffer, 0);

	// Render buffer object
	glGenRenderbuffers(1, &m_depthStencilBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, nmrSamples, GL_DEPTH24_STENCIL8, 800, 600);

	// Attach RBO to FB
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilBuffer);

	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER::NOT_COMPLETE\n";

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MSFramebuffer::Bind(GLenum target) const {
	glBindFramebuffer(target, Id);
}

const std::uint32_t MSFramebuffer::GetColorBuffer() const {
	return m_colorBuffer;
}

