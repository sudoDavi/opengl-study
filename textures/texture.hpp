#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "stb_image.h"
#include <glad/glad.h>

#include <cstdint>
#include <string>
#include <iostream>

class Texture {
private:
	std::uint32_t m_glTexture{};
public:
	Texture() = delete;
	Texture(const std::string& path, bool flipY = false, GLenum wrap = GL_REPEAT, int desiredChannels = 0);

// Public member variables
	int width{};
	int height{};
	int numberOfChannels{};

	// Bind the texture for usage
	void bind(GLenum unit) const;
};

#endif
