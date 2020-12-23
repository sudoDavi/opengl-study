#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdlib>

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);	
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	static bool wireframeMode{ false };
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		wireframeMode = !wireframeMode;

	if (wireframeMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		std::cout << "Set to wireframe\n";
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		std::cout << "Set to fill\n";
	}
}

// Check if shader compilation was successful
bool compilationSuccess(unsigned int shader) {
	int success{};
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cout << "ERROR: SHADER COMPILATION FAILED\n" << infoLog << '\n';
		return false;
	}
	else return true;
}

bool linkingSuccess(unsigned int shaderProgram) {
	int success{};
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR: SHADER LINKING FAILED\n" << infoLog << '\n';
		return false;
	}
	else return true;
}

int main() {
	// Boilerplate to open a window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Added for Mac OS X
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window{ glfwCreateWindow(800, 600, "LearnOpenGL - Hello Rectangle", nullptr, nullptr) };
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window\n" ;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD\n";
		glfwTerminate();
		return 1;
	}
	// End of Boilerplate

	// Assign a viewport of size 800x600
	glViewport(0, 0, 800, 600);
	// Assign a callback function for when the screen is resized
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// Rectangle vertices for the vertex shader (should probably create a vert struct later)
	float triangleVertices[]{
		// First triangle
		-0.5f, -0.5f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,

		// Second Triangle
		0.5f, -0.5f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f
	};

	// Creating a Vertex Array Object
	unsigned int VAO{};
	glGenVertexArrays(1, &VAO);
	// Bind the VAO
	glBindVertexArray(VAO);

	// Create a Vertex Buffer Object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	// Bind the VBO to the array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Input the vertex data into the array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

	// Linking the vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// GLSL Vertex Shader Source
	constexpr char *vertexShaderSource{ "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main() {\n"
			"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0"
	};

	// Compiling the vertex shader
	unsigned int vertexShader{ glCreateShader(GL_VERTEX_SHADER) };
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	// Check if the compilation was successful
	if (!compilationSuccess(vertexShader)) {
		glfwTerminate();
		return 1;
	}

	// GLSL Fragment Shader Source
	constexpr char *fragmentShaderSource{ "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
			"FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
		"}\0"
	};

	// Compiling the frament shader
	unsigned int fragmentShader{ glCreateShader(GL_FRAGMENT_SHADER) };
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	// Check if the compilation was successful
	if (!compilationSuccess(fragmentShader)) {
		glfwTerminate();
		return 1;
	}

	// Creating the default shader program
	unsigned int shaderProgram{ glCreateProgram() };
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check if the linking was successful
	if (!linkingSuccess(shaderProgram)) {
		glfwTerminate();
		return 1;
	}

	// Deleting the shader objects after linking
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Activate the shader
	glUseProgram(shaderProgram);

	// Bind a GLFW callback to change the drawing mode
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	// Main Rendering loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// CLEARS THE SCREEN
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw the bound array buffer
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
