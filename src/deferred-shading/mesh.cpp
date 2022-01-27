#include "mesh.hpp"

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<std::uint32_t> &indices, std::vector<ITexture> &textures)
	: Vertices{ vertices }, Indices{ indices }, Textures{ textures }
	{
		setupMesh();
	}

void Mesh::setupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(std::uint32_t), &Indices[0], GL_STATIC_DRAW);
	
	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

void Mesh::Draw(Shader &shader) {
	std::uint32_t diffuseNr{ 1 };
	std::uint32_t specularNr{ 1 };

	shader.use();

	for (auto index{ 0 }; index < Textures.size(); ++index) {
		glActiveTexture(GL_TEXTURE0 + index); // Activate proper texture unit before binding
		std::string number;
		std::string name = Textures[index].Type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		shader.setVec1f("material." + name + number, index);
		glBindTexture(GL_TEXTURE_2D, Textures[index].Id);
	}

	glActiveTexture(GL_TEXTURE0);

	// Actually Draw the mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::Draw() {
	// Actually Draw the mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
