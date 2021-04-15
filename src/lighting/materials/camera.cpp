#include "camera.hpp"

Camera::Camera(const Camera::vec3& position, const Camera::vec3& worldUp, float yaw, float pitch)
	: Position{position}, WorldUp{worldUp}, Yaw{yaw}, Pitch{pitch}, Target{ Camera::DefaultTarget }
	{
		UpdateCameraVectors();	
}

void Camera::UpdateCameraVectors() {
	Camera::vec3 _target;
	_target.x = std::cos(glm::radians(Yaw)) * std::cos(glm::radians(Pitch));
	_target.y = std::sin(glm::radians(Pitch));
	_target.z = std::sin(glm::radians(Yaw)) * std::cos(glm::radians(Pitch));
	Target = glm::normalize(_target);

	// Re-calculate the Right and Up vectors
	Right = glm::normalize(glm::cross(Target, WorldUp));
	Up = glm::normalize(glm::cross(Right, Target));
}

glm::mat4 Camera::GetLookAt() const {
	return glm::lookAt(Position, Position + Target, Up);
}

void Camera::ChangeZoom(float yoffset) {
	Zoom -= yoffset;
	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 120.0f)
		Zoom = 120.0f;
}
