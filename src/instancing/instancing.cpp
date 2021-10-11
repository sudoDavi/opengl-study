#include "constants.hpp"
#include "shader.hpp"
#include "flycam.hpp"
#include "texture.hpp"
#include "framebuffer.hpp"
#include "model.hpp"

#include <stb_image.h>
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
#include <vector>
#include <map>


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

	// Bind a GLFW callback to change the drawing mode
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	// Capture the mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Bind callback to Scroll Input
	glfwSetScrollCallback(window, scrollHandle);

	// Asteroids
	std::uint32_t amount{ 10000 };
	glm::mat4 *modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); // Initialize rng
	float radius{ 40.0f };
	float offset{ 10.5f };
	for (auto i{0}; i < amount; ++i) {
		glm::mat4 model = glm::mat4(1.0f);

		// Translation : displace along circle with radius in range [-offset, offset]
		float angle{ (float)i / (float)amount * 360.0f };
		float displacement{ (rand() % (int)(2 * offset * 100)) / 100.0f - offset };
		float x{ sin(angle) * radius + displacement };
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y{ displacement * 0.4f }; // keep height of field smaller compared to the size of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z{ cos(angle) * radius + displacement };

		model = glm::translate(model, glm::vec3(x, y, z));

		// Scale
		float scale{ (rand() % 20) / 100.0f + 0.05f };
		model = glm::scale(model, glm::vec3(scale));

		// Rotation
		float rotAngle{ rand() % 360 };
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		modelMatrices[i] = model;
	}

	// Models
	Model asteroid{ "assets/rock/rock.obj" };
	Model planet{ "assets/planet/planet.obj" };

	// Instancing Data
	std::uint32_t instancingBuffer;
	glGenBuffers(1, &instancingBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, instancingBuffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	for (auto i{0}; i < asteroid.meshes.size(); ++i) {
		auto VAO{ asteroid.meshes[i].VAO };
		glBindVertexArray(VAO);
		
		auto vec4Size = sizeof(glm::vec4);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
		

	// Shaders
	Shader shader{ "shaders/basic-shader.vert", "shaders/basic-shader.frag" };
	Shader instancingShader{ "shaders/instancing-shader.vert", "shaders/basic-shader.frag" };

	// Depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float deltaTime{};
	float lastFrame{ glfwGetTime() };

	// Main Rendering loop
	while (!glfwWindowShouldClose(window)) {
		float currentFrame{ glfwGetTime() };
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input handling
		processInput(window);

		movement(window, deltaTime, camera);
		mouseDirection(window, camera);

		// Create matrices
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom()), 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 view = camera.GetLookAt();
		glm::mat4 model = glm::mat4(1.0f);

		// CLEARS THE SCREEN
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw objects
		glEnable(GL_DEPTH_TEST);
		
		// Drawing planet
		shader.use();
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		shader.setMatrix4f("model", model);
		shader.setMatrix4f("view", view);
		shader.setMatrix4f("projection", projection);
		planet.Draw(shader);

		// Drawing the instanced asteroids
		instancingShader.use();
		instancingShader.setMatrix4f("view", view);
		instancingShader.setMatrix4f("projection", projection);
		instancingShader.setVec1i("texture_diffuse1", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, asteroid.textures_loaded[0].Id);
		for (auto i{0}; i < asteroid.meshes.size(); ++i) {
			glBindVertexArray(asteroid.meshes[i].VAO);
			glDrawElementsInstanced(
				GL_TRIANGLES, asteroid.meshes[i].Indices.size(), GL_UNSIGNED_INT, 0, amount
				);	
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Application will close, so we need to clean up
	delete[] modelMatrices;

	glfwTerminate();
	return 0;
}
