#include "shader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "texture.hpp"
#include "camera.hpp"
#include "constants.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdlib>
#include <cmath>

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);	
}

float getVisibility(GLFWwindow *window) {
	static float visibility{ 0.2f };
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		visibility += 0.05f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		visibility -= 0.05f;

	return visibility;
}

bool shouldRotate(GLFWwindow *window) {
	static bool rotating{ false };
	static int delay{};
	++delay;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && delay > 10) {
		rotating = !rotating;
		delay = 0;
	}

	return rotating;
}

bool changeFov(GLFWwindow *window) {
	static bool fov{ false };
	static int delay{};
	++delay;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && delay > 10) {
		fov = !fov;
		delay = 0;
	}

	return fov;	
}

bool changeAspect(GLFWwindow *window) {
	static bool aspect{ false };
	static int delay{};
	++delay;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && delay > 10) {
		aspect = !aspect;
		delay = 0;
	}

	return aspect;	
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

glm::vec3 movement(GLFWwindow* window, float deltaTime) {
	static float x{}, y{}, z{};
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		z += 0.1f * deltaTime * Config::MovementSpeed;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		z -= 0.1f * deltaTime * Config::MovementSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		x += 0.1f * deltaTime * Config::MovementSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		x -= 0.1f * deltaTime * Config::MovementSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		y -= 0.1f * deltaTime * Config::MovementSpeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		y += 0.1f * deltaTime * Config::MovementSpeed;

	return glm::vec3(x, y, z);
}

int main() {
	// Boilerplate to open a window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Added for Mac OS X
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window{ glfwCreateWindow(800, 600, "LearnOpenGL - Shader class exercises", nullptr, nullptr) };
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

	// CUBE vertices for the vertex shader
	float cubeVertices[] = {
	    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[]{
		glm::vec3( 0.0f,  0.0f,  0.0f),
		glm::vec3( 2.0f,  5.0f, -15.0f), 
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3( 2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3( 1.3f, -2.0f, -2.5f),
		glm::vec3( 1.5f,  2.0f, -2.5f),
		glm::vec3( 1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f) 
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	// Vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// COMMENTED OUT BECAUSE I'M NOT USING VERTEX COLOR
	// Vertex color attribute
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	// glEnableVertexAttribArray(1);
	// Texture coords attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Create a new shader using the Shader Class
	Shader defaultShader{ "shaders/shader.vert", "shaders/shader.frag" };
	defaultShader.use();
	defaultShader.setVec1i("texture1", 0);
	defaultShader.setVec1i("texture2", 1);
	
	// Bind a GLFW callback to change the drawing mode
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	// Load the container texture
	Texture container{ "assets/container.jpeg", false };
	Texture awesomeFace{ "assets/awesomeface.png", true };


	glEnable(GL_DEPTH_TEST);

	float deltaTime{};
	float lastFrame{ glfwGetTime() };

	// Main Rendering loop
	while (!glfwWindowShouldClose(window)) {
		float currentFrame{ glfwGetTime() };
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		// Update Transforms
		glm::mat4 model{ glm::mat4(1.0f) };

		glm::mat4 view{ glm::mat4(1.0f) };
		// Uncomment the line to select the right view, one keeps moving the camera back and forth, the other is static
		// if you don't uncomment , the camera is going to be inside the first box
		// view = glm::translate(view, glm::vec3(0.0f, 0.0f, static_cast<float>(std::abs(std::sin(glfwGetTime())) * -6.0f) - 0.5f));
		Camera fpsCam{ movement(window, deltaTime)};
		view = fpsCam.lookAt();
		// Implements the movement of the Camera Position, NOT THE TARGET
		// view = glm::lookAt(movement(window), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		// IMPLEMENTS A ROTATING CAMERA AROUND THE MIDDLE BLOCK
		// float radius = 10.0f;
        // float camX   = sin(glfwGetTime()) * radius;
        // float camZ   = cos(glfwGetTime()) * radius;
        // view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		auto fov{changeFov(window)};
		auto aspect{changeAspect(window)};
		
		glm::mat4 projection{ glm::mat4(1.0f) };
		if (aspect && fov)
			projection = glm::perspective(static_cast<float>(glfwGetTime()), std::sin(static_cast<float>(glfwGetTime())), 0.1f, 100.0f);
		else if (fov)
			projection = glm::perspective(static_cast<float>(glfwGetTime()), 800.0f / 600.0f, 0.1f, 100.0f);
		else if (aspect)
			projection = glm::perspective(glm::radians(45.0f), std::sin(static_cast<float>(glfwGetTime())), 0.1f, 100.0f);
		else
			projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

		// CLEARS THE SCREEN
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Get arrow key input
		auto smileyVisibility{ getVisibility(window) };

		// Draw the Rectangle
		defaultShader.use();
		defaultShader.setVec1f("texture2mix", smileyVisibility);
		defaultShader.setMatrix4f("view", view);
		defaultShader.setMatrix4f("projection", projection);
		container.bind(GL_TEXTURE0);
		awesomeFace.bind(GL_TEXTURE1);

		auto rotate{ shouldRotate(window) };

		for (auto index{0}; index < 10; ++index) {
			glm::mat4 model{glm::mat4(1.0f)};
			model = glm::translate(model, cubePositions[index]);
			if (rotate && !(index % 3))
				model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(0.5f, 1.0f, 0.5f));
			defaultShader.setMatrix4f("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
