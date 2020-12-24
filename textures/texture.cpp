#include "texture.hpp"

Texture::Texture(const std::string& path, int desiredChannels)
	: m_dataPtr{ stbi_load(path.c_str(), &width, &height, &numberChannels, desiredChannels) }
	{
	GLenum format;
    if (numberChannels == 1) {

      format = GL_RED;
    }
    else if (numberChannels == 3) {

      format = GL_RGB;
    }
    else if (numberChannels == 4) {

      format = GL_RGBA;
    }
	// Generate the OpenGL texture ID
	glGenTextures(1, &m_glTexture);
	// Bind the generated ID to the currently used OpenGL Texture
	glBindTexture(GL_TEXTURE_2D, m_glTexture);
	// Configure the wrapping and filtering options
		// Wrapping for the X axis of the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		// Wrapping for the Y axis of the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Filtering for minifying the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// Filtering for magnifying the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// End of configuration

	// Check if loading was successful
	if (m_dataPtr) {
		// Load data and generate mipmaps
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_dataPtr);
		glGenerateMipmap(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_glTexture);
	} else {
		std::cerr << "ERROR::TEXTURE::FAILURE_TO_LOAD\n";
	}

	// Free the image memory
	stbi_image_free(m_dataPtr);
}

void Texture::bind() const {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_glTexture);
}
