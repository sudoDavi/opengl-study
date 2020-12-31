#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "constants.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

class Camera {
public: 
	// Type aliases
	using vec3 = glm::vec3;
	// Static Default Values
	static constexpr vec3 DefaultVector{ 0.0f, 0.0f, 0.0f };
	static constexpr vec3 DefaultWorldUp{ 0.0f, 1.0f, 0.0f };
	static constexpr vec3 DefaultTarget{ 0.0f, 0.0f, -1.0f };

public:	// Public Member variables
	// Camera Vectors
	vec3 Position{};
	vec3 Target{};
	vec3 Up{};
	vec3 Right{};
	vec3 WorldUp{};
	// Camera Angles
	float Yaw;
	float Pitch;
	// Camera Options
	float Zoom{ CameraConfig::DefaultZoom };

public:
	// Nested Types
	// Define Platform independent Movement Options
	enum Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	// Public Member Functions
	// Default Constructor for the base camera
	Camera(const vec3& position = DefaultVector, const vec3& worldUp = DefaultWorldUp, float yaw = CameraConfig::DefaultYaw, float pitch = CameraConfig::DefaultPitch);
	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw = CameraConfig::DefaultYaw, float pitch = CameraConfig::DefaultPitch);

	// Get the LookAt matrix of this camera
	glm::mat4 GetLookAt() const;
	// Change the Zoom of the camera
	void ChangeZoom(float yoffset);

	// Calculate the Target Vector, using the camera Euler Angles
	void UpdateCameraVectors();
};
#endif
