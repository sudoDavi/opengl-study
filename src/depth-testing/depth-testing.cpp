#include "constants.hpp"
#include "shader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "flycam.hpp"
#include "model.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <string>


// I know it is a global variable, but I'm just using it because I haven't implemented a Game Object yet
// and it's just so I can use the scrool for the zoom
FlyCam camera{ glm::vec3(0.0f, 0.0f, 3.0f) };

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

void movement(GLFWwindow* window, float deltaTime, FlyCam& camera) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.Move(Camera::Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
       camera.Move(Camera::Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.Move(Camera::Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.Move(Camera::Movement::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.Move(Camera::Movement::UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.Move(Camera::Movement::DOWN, deltaTime);
}

void mouseDirection(GLFWwindow* window, FlyCam& camera) {
	static float lastX{ 400.0f }, lastY{ 300.0f };
	double xpos{}, ypos{};
	glfwGetCursorPos(window, &xpos, &ypos);

	float xoffset{ xpos - lastX };
	float yoffset{ lastY - ypos };
	lastX = xpos;
	lastY = ypos;

	camera.PointAt(xoffset, yoffset);
}

void scrollHandle(GLFWwindow* window, double xoffset, double yoffset) {
	std::cout << yoffset << '\n';
	camera.ChangeZoom(yoffset);
}

int main() {
	// Boilerplate to open a window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	// Added for Mac OS X
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window{ glfwCreateWindow(800, 600, "LearnOpenGL Tutorials", nullptr, nullptr) };
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

	Shader basicShader { "shaders/basic-shader.vert", "shaders/basic-shader.frag" };

	stbi_set_flip_vertically_on_load(true);

	Model backpack { "assets/backpack.obj" };

	glm::vec3 objectPositions[]{
		glm::vec3(2.0f, 1.0f, 0.0f),
		glm::vec3(5.0f, 10.0f, 5.0f),
		glm::vec3(-4.0f, -2.0f, -1.0f)
	};

		
	// Bind a GLFW callback to change the drawing mode
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	// Capture the mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Bind callback to Scroll Input
	glfwSetScrollCallback(window, scrollHandle);


	glEnable(GL_DEPTH_TEST);

	float deltaTime{};
	float lastFrame{ glfwGetTime() };

	// Main Rendering loop
	while (!glfwWindowShouldClose(window)) {
		float currentFrame{ glfwGetTime() };
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input handling
		processInput(window);

		// Update Transforms

		glm::mat4 view{ glm::mat4(1.0f) };

	        movement(window, deltaTime, camera);
        	mouseDirection(window, camera);
	        view = camera.GetLookAt();
		
		glm::mat4 projection{ glm::mat4(1.0f) };
		
		projection = glm::perspective(glm::radians(camera.Zoom()), 800.0f / 600.0f, 0.1f, 100.0f);

		// CLEARS THE SCREEN
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		basicShader.use();
		basicShader.setMatrix4f("projection", projection);
		basicShader.setMatrix4f("view", view);

		for (auto index{0}; index < 3; ++index) {
			glm::mat4 model { glm::mat4(1.0f) };
			model = glm::translate(model, objectPositions[index]);
			model = glm::scale(model, glm::vec3(1.0f));

			basicShader.setMatrix4f("model", model);
			backpack.Draw(basicShader);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
