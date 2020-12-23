#include "shader.hpp"

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
	: Id{ glCreateProgram() }
	{
	std::string vertexCode{};
	std::string fragmentCode{};
	std::ifstream vShaderFile{};
	std::ifstream fShaderFile{};

	// Ensure ifstream objects can throw exceptions
	vShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	fShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

	try {
		// Open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream{}, fShaderStream{};
		// Read the files buffer into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// Close the files
		vShaderFile.close();
		fShaderFile.close();

		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	} catch (std::ifstream::failure &error) {
		std::cerr << "ERROR::SHADER::FAILURE_TO_READ_FILE\n";
		std::cerr << "Error Code: " << error.code() << " Error Info: " << error.what() << '\n';
	}

	// Create CStrings with the source
	const char* vertexShaderCode{ vertexCode.c_str()  };
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

void Shader::setBool(const std::string &name, bool value) const {
	glUniform1i(glGetUniformLocation(Id, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string &name, std::int32_t value) const {
	glUniform1i(glGetUniformLocation(Id, name.c_str()), value);	
}

void Shader::setFloat(const std::string &name, float value) const {
	glUniform1f(glGetUniformLocation(Id, name.c_str()), value);
}
