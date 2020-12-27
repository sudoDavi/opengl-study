#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

class Camera {
private:
	static constexpr glm::vec3 m_worldUp{ glm::vec3(0.0f, 1.0f, 0.0f) };
	glm::vec3 m_cameraPos{};
	glm::vec3 m_cameraFront{};
	glm::vec3 m_cameraUp{};
	

public:
	Camera(const glm::vec3& cameraPos = glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 lookAt();
};

#endif
