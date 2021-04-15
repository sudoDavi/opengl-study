#ifndef FPSCAM_HPP
#define FPSCAM_HPP

#include "constants.hpp"
#include "camera.hpp"

class FpsCam {
public: // Camera Options
	float MouseSensitivity{ CameraConfig::DefaultSensitivity };
	float MovementSpeed{ FpsCamConfig::DefaultMovementSpeed };
	// Member Types
	using Movement = Camera::Movement;

private: // Member variables
	Camera m_Cam;
	Camera::vec3 m_front{};

	void updateCameraVectors();

public: // Public Member Functions
	FpsCam(const Camera::vec3& position = Camera::DefaultVector, const Camera::vec3& worldUp = Camera::DefaultWorldUp, float yaw = CameraConfig::DefaultYaw, float pitch = CameraConfig::DefaultPitch);
	FpsCam(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw = CameraConfig::DefaultYaw, float pitch = CameraConfig::DefaultPitch);	

	void Move(Movement direction, float deltaTime);
	void PointAt(float xoffset, float yoffset, bool constrainPitch = true);
	void ChangeZoom(float yoffset);

	float Zoom() { return m_Cam.Zoom; }

	glm::mat4 GetLookAt() const;
};

#endif
