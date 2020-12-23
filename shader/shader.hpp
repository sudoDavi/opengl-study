#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdint>

class Shader {
private:
	// Verify the compilation of both the vertex and fragment shaders
	// RETURNS TRUE if compilation was a SUCCESS
	bool verifyCompilation(std::uint32_t vertexShaderId, std::uint32_t fragmentShaderId);
	// Verify the linking of the shader program
	// RETURNS TRUE if linking was a SUCCESS
	bool verifyLinking();
public:
	// Shader Program ID
	std::uint32_t Id{};
	// Read and build the shader
	Shader(const std::string &vertexPath, const std::string &fragmentPath);
	// Activate the shader
	void use() const;
	// utility uniform functions
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, std::int32_t value) const;
	void setFloat(const std::string &name, float value) const;
};

#endif
