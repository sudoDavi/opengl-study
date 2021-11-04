#include "fpscam.hpp"

FpsCam::FpsCam(const Camera::vec3& position, const Camera::vec3& worldUp, float yaw, float pitch)
	: m_Cam{ position, worldUp, yaw, pitch }
	{
		updateCameraVectors();		
}

void FpsCam::Move(Movement direction, float deltaTime) {
	float velocity{ MovementSpeed * deltaTime };

	switch (direction) {
		case Movement::FORWARD :
			m_Cam.Position += m_front * velocity;
			break;
		case Movement::BACKWARD :
			m_Cam.Position -= m_front * velocity;
			break;
		case Movement::LEFT :
			m_Cam.Position -= m_Cam.Right * velocity;
			break;
		case Movement::RIGHT :
			m_Cam.Position += m_Cam.Right * velocity;
			break;
		default:
			break;
	}
}

void FpsCam::PointAt(float xoffset, float yoffset, bool constrainPitch) {
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	m_Cam.Yaw += xoffset;
	m_Cam.Pitch += yoffset;

	if (constrainPitch) {
		if (m_Cam.Pitch > 89.0f)
			m_Cam.Pitch = 89.0f;
		if (m_Cam.Pitch < -89.0f)
			m_Cam.Pitch = -89.0f;
	}

	updateCameraVectors();
}

void FpsCam::ChangeZoom(float yoffset) {
	m_Cam.ChangeZoom(yoffset);
}

glm::mat4 FpsCam::GetLookAt() const {
	return m_Cam.GetLookAt();
}

void FpsCam::updateCameraVectors() {
	Camera::vec3 _target;
	_target.x = std::cos(glm::radians(m_Cam.Yaw)) * std::cos(glm::radians(m_Cam.Pitch));
	_target.y = std::sin(glm::radians(m_Cam.Pitch));
	_target.z = std::sin(glm::radians(m_Cam.Yaw)) * std::cos(glm::radians(m_Cam.Pitch));
	m_Cam.Target = glm::normalize(_target);

	// Re-calculate the Right and Up vectors
	m_Cam.Right = glm::normalize(glm::cross(m_Cam.Target, m_Cam.WorldUp));
	m_front = glm::normalize(glm::cross(m_Cam.WorldUp, m_Cam.Right));
}
