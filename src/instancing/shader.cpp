#include "shader.hpp"

const std::string readFile(const std::string& path) {
	std::ifstream file{};
	
	file.exceptions(std::ifstream::badbit | std::ifstream::failbit);

	try {
		// Open file
		file.open(path);

		std::stringstream fileStream{};
		
		// Read the file buffer into stream
		fileStream << file.rdbuf();
		
		// Close the file
		file.close();

		// Return the string stream as a string
		return std::string(fileStream.str());
	} catch (std::ifstream::failure &error) {
		std::cerr << "ERROR::FILE::FAILURE_TO_READ_FILE\n";
		std::cerr << "Error Code: " << error.code() << " Error Info: " << error.what() << std::endl;

		file.close();

		return "";
	}
}

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
	: Id{ glCreateProgram() }
	{
	std::string vertexCode{ readFile(vertexPath) };
	std::string fragmentCode{ readFile(fragmentPath) };
	// Create CStrings with the source
	const char* vertexShaderCode{ vertexCode.c_str() };
	const char* fragmentShaderCode{ fragmentCode.c_str()  };

	// Create the Ids for the shaders
	std::uint32_t vertexShaderId{ glCreateShader(GL_VERTEX_SHADER) }, fragmentShaderId{ glCreateShader(GL_FRAGMENT_SHADER) };

	// Assign the sources and compile
	// Vertex shader
	glShaderSource(vertexShaderId, 1, &vertexShaderCode, nullptr);
	glCompileShader(vertexShaderId);
	// Fragment shader
	glShaderSource(fragmentShaderId, 1, &fragmentShaderCode, nullptr);
	glCompileShader(fragmentShaderId);

	// Call private function verifyCompilation()
	// it verifies the compilation of both shaders
	verifyCompilation(vertexShaderId, fragmentShaderId);

	// Attach the shaders into the Shader Program
	glAttachShader(Id, vertexShaderId);
	glAttachShader(Id, fragmentShaderId);
	// Link the Program
	glLinkProgram(Id);

	// Verify the linking
	verifyLinking();

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
}

Shader::Shader(const std::string &vertexPath,const std::string &geometryPath, const std::string &fragmentPath)
	: Id{ glCreateProgram() }, m_hasGeometryShader{ true }
	{
	std::string vertexCode{ readFile(vertexPath) };
	std::string geometryCode{ readFile(geometryPath) };
	std::string fragmentCode{ readFile(fragmentPath) };

	// Create CStrings with the source
	const char* vertexShaderCode{ vertexCode.c_str() };
	const char* geometryShaderCode{ geometryCode.c_str() };
	const char* fragmentShaderCode{ fragmentCode.c_str()  };

	// Create the Ids for the shaders
	std::uint32_t vertexShaderId{ glCreateShader(GL_VERTEX_SHADER) },
		geometryShaderId{ glCreateShader(GL_GEOMETRY_SHADER) },
		fragmentShaderId{ glCreateShader(GL_FRAGMENT_SHADER) };

	// Assign the sources and compile
	// Vertex shader
	glShaderSource(vertexShaderId, 1, &vertexShaderCode, nullptr);
	glCompileShader(vertexShaderId);
	// Geometry shader
	glShaderSource(geometryShaderId, 1, &geometryShaderCode, nullptr);
	glCompileShader(geometryShaderId);
	// Fragment shader
	glShaderSource(fragmentShaderId, 1, &fragmentShaderCode, nullptr);
	glCompileShader(fragmentShaderId);

	// Call private function verifyCompilation()
	// it verifies the compilation of all shaders
	verifyCompilation(vertexShaderId, geometryShaderId, fragmentShaderId);

	// Attach the shaders into the Shader Program
	glAttachShader(Id, vertexShaderId);
	glAttachShader(Id, geometryShaderId);
	glAttachShader(Id, fragmentShaderId);
	// Link the Program
	glLinkProgram(Id);

	// Verify the linking
	verifyLinking();

	glDeleteShader(vertexShaderId);
	glDeleteShader(geometryShaderId);
	glDeleteShader(fragmentShaderId);
}



bool Shader::verifyCompilation(std::uint32_t vertexShaderId, std::uint32_t fragmentShaderId) {
	int success{};
	char infoLog[512];

	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShaderId, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
		return false;
	}

	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderId, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << '\n';
		return false;
	}

	return true;
}

bool Shader::verifyCompilation(std::uint32_t vertexShaderId, std::uint32_t geometryShaderId, std::uint32_t fragmentShaderId) {
	int success{};
	char infoLog[512];

	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShaderId, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
		return false;
	}

	glGetShaderiv(geometryShaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(geometryShaderId, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << '\n';
		return false;
	}

	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderId, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << '\n';
		return false;
	}

	return true;
}

bool Shader::verifyLinking() {
	int success{};
	char infoLog[512];
	glGetProgramiv(Id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(Id, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
		return false;
	}
	else return true;
}

void Shader::use() const {
	glUseProgram(Id);
}

void Shader::setVec1b(const std::string &name, bool value) const {
	glUniform1i(glGetUniformLocation(Id, name.c_str()), static_cast<int>(value));
}

void Shader::setVec1i(const std::string &name, std::int32_t value) const {
	glUniform1i(glGetUniformLocation(Id, name.c_str()), value);	
}

void Shader::setVec1f(const std::string &name, float value) const {
	glUniform1f(glGetUniformLocation(Id, name.c_str()), value);
}

void Shader::setVec3f(const std::string &name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(Id, name.c_str()), x, y, z);
}

void Shader::setVec3f(const std::string &name, const glm::vec3 &vector) const {
	glUniform3f(glGetUniformLocation(Id, name.c_str()), vector.x, vector.y, vector.z);
}

void Shader::setMatrix4f(const std::string &name, const glm::mat4 &matrix) const {
	glUniformMatrix4fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVec2f(const std::string &name, float x, float y) const {
	glUniform2f(glGetUniformLocation(Id, name.c_str()), x, y);
}

void Shader::setVec2f(const std::string &name, const glm::vec2 &vector) const {
	glUniform2f(glGetUniformLocation(Id, name.c_str()), vector.x, vector.y);
}
