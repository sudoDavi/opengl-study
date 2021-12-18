#include "texture.hpp"

Texture::Texture(const std::string& path, bool flipY, GLenum wrap, int desiredChannels, bool gammaCorrection)
	{
	// Configure Load options
	stbi_set_flip_vertically_on_load(flipY);

	// Load the image
	int _width, _height, _numberChannels;
	unsigned char *dataPtr{ stbi_load(path.c_str(), &_width, &_height, &_numberChannels, desiredChannels) };

	// Copy the variables to public member ones
	width = _width;
	height = _height;
	numberOfChannels = _numberChannels;

	GLenum format, internalFormat;
	if (_numberChannels == 1) {
		internalFormat = GL_RED;
		format = GL_RED;
	}
	else if (_numberChannels == 3) {
		internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
		format = GL_RGB;
	}
	else if (_numberChannels == 4) {
		internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
		format = GL_RGBA;
	}
	// Generate the OpenGL texture ID
	glGenTextures(1, &m_glTexture);
	// Bind the generated ID to the currently used OpenGL Texture
	glBindTexture(GL_TEXTURE_2D, m_glTexture);
	// Configure the wrapping and filtering options
		// Wrapping for the X axis of the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		// Wrapping for the Y axis of the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
		// Filtering for minifying the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// Filtering for magnifying the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// End of configuration

	// Check if loading was successful
	if (dataPtr) {
		// Load data and generate mipmaps
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, dataPtr);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cerr << "ERROR::TEXTURE::FAILURE_TO_LOAD\n";
	}

	// Free the image memory
	stbi_image_free(dataPtr);
}

void Texture::bind(GLenum unit) const {
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, m_glTexture);
}
