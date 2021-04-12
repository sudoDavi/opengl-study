#include "constants.hpp"
#include "shader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "texture.hpp"
#include "flycam.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>


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

	// CUBE vertices for the vertex shader
	float cubeVertices[] = {
	    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
	     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
	     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
	    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
	    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
	
	    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	
	    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	
	     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	
	    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	
	    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	glm::vec3 cubePosition(0.0f, 0.0f, 0.0f);


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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// We don't need this part, since the current vertex data doesn't include UV mapping
	// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// glEnableVertexAttribArray(2);

	// Create a new VAO for the light object
	unsigned int lightVAO{};
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We can re-use the same VBO, since we're doing a cube light object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Re-configure the VertexAttributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// Create the shader that's used to light up the Cube
	Shader lightingShader{ "shaders/lighting.vert", "shaders/lighting.frag" };
	lightingShader.use();
	lightingShader.setVec3f("objectColor", 1.0f, 0.5f, 0.31f);

	// Shader for the light source cube object i.e Lamp
	Shader lightObjectShader{ "shaders/light.vert", "shaders/light.frag" };
	// Position of the light source in the world
	glm::vec3 lightOffset(0.0f);
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	
	// Bind a GLFW callback to change the drawing mode
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	// Capture the mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Bind callback to Scroll Input
	glfwSetScrollCallback(window, scrollHandle);

	// Load the container texture
	Texture container{ "assets/container.jpeg", false };
	Texture awesomeFace{ "assets/awesomeface.png", true };


	glEnable(GL_DEPTH_TEST);

	float deltaTime{};
	float lastFrame{ glfwGetTime() };

	// Boolean to tell the program to render or not render the LIGHT OBJECT (cube)
	bool hideLight{ false };


	// Main Rendering loop
	while (!glfwWindowShouldClose(window)) {
		float currentFrame{ glfwGetTime() };
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		//Modify the color of the light
		// float sinOfTime{ std::abs(std::sin(currentFrame)) };
		// lightColor = glm::vec3(sinOfTime);

		// Modify the lightPosition to the camera's position
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			lightPos = camera.GetPosition();
			hideLight = true;
		}
		else
			hideLight = false;

		// Make the light rotate around the scene if the key R was pressed
		if (shouldRotate(window))
		{
			lightOffset.x = sin(currentFrame) + lightPos.x;
			lightOffset.z = cos(currentFrame) + lightPos.z;
			lightOffset.y = lightPos.y;
		}
		else
		{
			lightOffset = lightPos;
		}

		// Update Transforms

		glm::mat4 view{ glm::mat4(1.0f) };

        movement(window, deltaTime, camera);
        mouseDirection(window, camera);
        view = camera.GetLookAt();
		
		glm::mat4 projection{ glm::mat4(1.0f) };
		
		projection = glm::perspective(glm::radians(camera.Zoom()), 800.0f / 600.0f, 0.1f, 100.0f);

		// CLEARS THE SCREEN
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Get arrow key input
		auto smileyVisibility{ getVisibility(window) };

		// Draw the Cube
		glBindVertexArray(VAO);
		lightingShader.use();
		lightingShader.setMatrix4f("view", view);
		lightingShader.setMatrix4f("projection", projection);
		lightingShader.setVec3f("light.position", lightOffset);
		lightingShader.setVec3f("light.ambient", 0.2f, 0.2f, 0.2f);
		lightingShader.setVec3f("light.diffuse", 0.5f, 0.5f, 0.5f);
		lightingShader.setVec3f("light.specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3f("viewPos", camera.GetPosition());
		lightingShader.setVec3f("material.ambient", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3f("material.diffuse", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3f("material.specular", 0.5f, 0.5f, 0.5f);
		lightingShader.setVec1f("material.shininess", 32.0f);
		
		glm::mat4 model{glm::mat4(1.0f)};
		model = glm::translate(model, cubePosition);
		lightingShader.setMatrix4f("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Draw the light source cube
		if (!hideLight)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, lightOffset);
			model = glm::scale(model, glm::vec3(0.2f));
			lightObjectShader.use();
			lightObjectShader.setMatrix4f("view", view);
			lightObjectShader.setMatrix4f("projection", projection);
			lightObjectShader.setMatrix4f("model", model);
			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);	
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
