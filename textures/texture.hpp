#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "stb_image.h"
#include <glad/glad.h>

#include <cstdint>
#include <string>
#include <iostream>

class Texture {
private:
	unsigned char *m_dataPtr{};
	std::uint32_t m_glTexture{};
public:
	Texture() = delete;
	Texture(const std::string& path, int desiredChannels = 0);

// Public member variables
	int width{};
	int height{};
	int numberChannels{};

	// Bind the texture for usage
	void bind() const;
};

#endif
