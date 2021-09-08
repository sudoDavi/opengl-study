#include "model.hpp"

Model::Model(std::string &path) {
	loadModel(path);
}

void Model::Draw(Shader &shader) {
	for (auto index{ 0 }; index < meshes.size(); ++index)
		meshes[index].Draw(shader);
}

void Model::loadModel(std::string &path) {
	Assimp::Importer import;
	const aiScene *scene{ import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs) };

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
	// process all the node's meshes (if any)
	for (auto index {0}; index < node->mNumMeshes; ++index) {
		aiMesh *mesh{ scene->mMeshes[node->mMeshes[index]] };
		meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (auto index {0}; index < node->mNumChildren; ++index) {
		processNode(node->mChildren[index], scene);
	}
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, const aiTextureType &type, const std::string &typeName) {
	std::vector<Texture> textures;
	for (auto index{0}; index < mat->GetTextureCount(type); ++index) {
		aiString str;
		mat->GetTexture(type, index, &str);

		bool skipLoading { false };
		for (auto loadedIndex{0}; loadedIndex < textures_loaded.size(); ++loadedIndex) {
			if (std::strcmp(textures_loaded[loadedIndex].Path.data(), str.C_Str()) == 0) {
				textures.push_back(textures_loaded[loadedIndex]);
				skipLoading = true;
				break;
			}
		}
		if (!skipLoading) {
			Texture texture;
			texture.Id = TextureFromFile(str.C_Str(), directory);
			texture.Type = typeName;
			texture.Path = str;
			textures.push_back(texture);
		}
	}

	return textures;
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
	std::vector<Vertex> vertices;
	std::vector<std::uint32_t> indices;
	std::vector<Texture> textures;

	for (auto index {0}; index < mesh->mNumVertices; ++index) {
		glm::vec3 position { mesh->mVertices[index].x, mesh->mVertices[index].y,
			mesh->mVertices[index].y };

		glm::vec3 normal { mesh->mNormals[index].x, mesh->mNormals[index].y,
			mesh->mNormals[index].z };
		
		glm::vec2 uvMap;
		if (mesh->mTextureCoords[0]) { // Does the mesh have UV mapping?
			uvMap = { mesh->mTextureCoords[0][index].x,
				mesh->mTextureCoords[0][index].y };
		}
		else
			uvMap = { 0.0f, 0.0f };


		vertices.push_back(Vertex(position, normal, uvMap));
	}

	for (auto index{0}; index < mesh->mNumFaces; ++index) {
		aiFace face { mesh->mFaces[index] };

		for (auto vertexIndex{0}; vertexIndex < face.mNumIndices; ++vertexIndex)
			indices.push_back(face.mIndices[vertexIndex]);
	}


	if (mesh->mMaterialIndex >= 0) {
		aiMaterial *material { scene->mMaterials[mesh->mMaterialIndex] };

		std::vector<Texture> diffuseMaps { loadMaterialTextures(material,
				aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps { loadMaterialTextures(material,
				aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}
