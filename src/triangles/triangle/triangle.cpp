#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdlib>
#include <array>
#include <cmath>

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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

void convertToOpenGL(double* xpos, double* ypos) {
	// This is somewhat of a bad example, because the resolution is hard coded
	double convertedX{ ((*xpos / 800) - 0.5) * 2 };
	double convertedY{ ((*ypos / 600) - 0.5) * 2 };
	*xpos = convertedX > 1.0 || convertedX < -1.0 ? 0.0 : convertedX;
	*ypos = convertedY > 1.0 || convertedY < -1.0 ? 0.0 : convertedY;
}

void checkAndChange(float* vertsArray, int lenght, float xpos, float ypos) {
	// This is also a bad example, since I'm hardcoding how the verts are placed in the array
	float smallestDistance{20.0f};
	float* smallestXpos;
	float* smallestYpos;
	for (int index{}; index < lenght - 1; index += 3) {
		float compareDistance{ std::abs(std::sqrt(std::pow(xpos - vertsArray[index], 2) + std::pow(ypos - vertsArray[index + 1], 2))) };
		if (compareDistance < smallestDistance) {
			smallestDistance = compareDistance;
			smallestYpos = &vertsArray[index + 1];
			smallestXpos = &vertsArray[index];
		}
	}

	*smallestXpos = xpos;
	*smallestYpos = ypos;
}


int main() {
	// Boilerplate to open a window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Added for Mac OS X
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window{ glfwCreateWindow(800, 600, "LearnOpenGL - Hello Triangle", nullptr, nullptr) };
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

	// Triangle vertices for the vertex shader (should probably create a vert struct later)
	float triangleVertices[]{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
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

	// Main Rendering loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		// Poll the mouse button state
		// Cool Idea for when I figure out how to fix
		// Currently moves the vertices in a very bad way
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			double xpos{}, ypos{};
			glfwGetCursorPos(window, &xpos, &ypos);
			convertToOpenGL(&xpos, &ypos);
			checkAndChange(triangleVertices, 9, xpos, ypos);
		}

		// CLEARS THE SCREEN
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
		// Draw the array, which is set by the VAO bound previously
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
