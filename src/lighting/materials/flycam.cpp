#include "flycam.hpp"

FlyCam::FlyCam(const Camera::vec3& position, const Camera::vec3& worldUp, float yaw, float pitch)
	: m_Cam{ position, worldUp, yaw, pitch }
	{
	}

void FlyCam::Move(Movement direction, float deltaTime) {
	float velocity{ MovementSpeed * deltaTime };

	switch (direction) {
		case Movement::FORWARD :
			m_Cam.Position += m_Cam.Target * velocity;
			break;
		case Movement::BACKWARD :
			m_Cam.Position -= m_Cam.Target * velocity;
			break;
		case Movement::LEFT :
			m_Cam.Position -= m_Cam.Right * velocity;
			break;
		case Movement::RIGHT :
			m_Cam.Position += m_Cam.Right * velocity;
			break;
		case Movement::UP :
			m_Cam.Position += m_Cam.Up * velocity;
			break;
		case Movement::DOWN :
			m_Cam.Position -= m_Cam.Up * velocity;
			break;
		default:
			break;
	}
}

void FlyCam::PointAt(float xoffset, float yoffset, bool constrainPitch) {
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

	m_Cam.UpdateCameraVectors();
}

void FlyCam::ChangeZoom(float yoffset) {
	m_Cam.ChangeZoom(yoffset);
}

glm::mat4 FlyCam::GetLookAt() const {
	return m_Cam.GetLookAt();
}

const glm::vec3& FlyCam::GetPosition() const {
	return m_Cam.Position;
}
