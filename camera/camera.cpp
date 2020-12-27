#include "camera.hpp"

Camera::Camera(const glm::vec3& cameraPos)
	: m_cameraPos{cameraPos}, m_cameraFront{glm::vec3(0.0f, 0.0f, -0.1f)}
	{
	m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::lookAt() {
	return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}
