#ifndef MESH_HPP
#define MESH_HPP

#include "shader.hpp"

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <vector>
#include <string>
#include <cstdint>
#include <cstddef>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;

	Vertex()
	{
	}
	Vertex(glm::vec3 &position, glm::vec3 &normal, glm::vec2 &texCoords)
		: Position{ position }, Normal{ normal }, TexCoords{ texCoords }
		{
		}
};

struct ITexture {
	std::uint32_t Id;
	std::string Type;
	std::string Path;
};


class Mesh {
	public:
		std::vector<Vertex> Vertices;
		std::vector<std::uint32_t> Indices;
		std::vector<ITexture> Textures;

		Mesh(std::vector<Vertex> &vertices, std::vector<std::uint32_t> &indices, std::vector<ITexture> &textures);
		void Draw(Shader& shader);
		void Draw();
	private:
		// OpenGL Data;
		std::uint32_t VAO, VBO, EBO;

		void setupMesh();
};

#endif
