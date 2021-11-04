#ifndef FLYCAM_HPP
#define FLYCAM_HPP

#include "constants.hpp"
#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class FlyCam {
public: // Camera Options
	float MouseSensitivity{ CameraConfig::DefaultSensitivity };
	float MovementSpeed{ FlyCamConfig::DefaultMovementSpeed };
	// Member Types
	using Movement = Camera::Movement;

private: // Member variables
	Camera m_Cam;

public: // Public Member Functions
	FlyCam(const Camera::vec3& position = Camera::DefaultVector, const Camera::vec3& worldUp = Camera::DefaultWorldUp, float yaw = CameraConfig::DefaultYaw, float pitch = CameraConfig::DefaultPitch);
	FlyCam(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw = CameraConfig::DefaultYaw, float pitch = CameraConfig::DefaultPitch);

	void Move(Movement direction, float deltaTime);
	void PointAt(float xoffset, float yoffset, bool constrainPitch = true);
	void ChangeZoom(float yoffset);

	float Zoom() { return m_Cam.Zoom; }

	glm::mat4 GetLookAt() const;
	const glm::vec3& GetPosition() const;
	const glm::vec3& GetTarget() const;
};

#endif
