#include "model.hpp"

Model::Model(const std::string &path) {
	loadModel(path);
}

void Model::Draw(Shader &shader) {
	for (auto index{ 0 }; index < meshes.size(); ++index)
		meshes[index].Draw(shader);
}

void Model::loadModel(const std::string &path) {
	Assimp::Importer import;
	const aiScene *scene{ import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_FlipUVs
			| aiProcess_CalcTangentSpace) };

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

std::uint32_t TextureFromFile(const char *path, const std::string &directory, bool gamma) {
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	std::uint32_t textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
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
		       	texture.Id = TextureFromFile(str.C_Str(), this->directory);
			texture.Type = typeName;
			texture.Path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}

	return textures;
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
	std::vector<Vertex> vertices;
	std::vector<std::uint32_t> indices;
	std::vector<Texture> textures;

	for (auto index{0}; index <mesh->mNumVertices; ++index) {
		Vertex vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[index].x;
		vector.y = mesh->mVertices[index].y;
		vector.z = mesh->mVertices[index].z;

		vertex.Position = vector;

		if (mesh->HasNormals()) {
			vector.x = mesh->mNormals[index].x;
			vector.y = mesh->mNormals[index].y;
			vector.z = mesh->mNormals[index].z;
			vertex.Normal = vector;
		}

		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][index].x;
			vec.y = mesh->mTextureCoords[0][index].y;
			vertex.TexCoords = vec;

			vector.x = mesh->mTangents[index].x;
			vector.y = mesh->mTangents[index].y;
			vector.z = mesh->mTangents[index].z;
			vertex.Tangent = vector;

			vector.x = mesh->mBitangents[index].x;
			vector.y = mesh->mBitangents[index].y;
			vector.z = mesh->mBitangents[index].z;
			vertex.Bitangent = vector;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	for(auto index{0}; index < mesh->mNumFaces; ++index) {
		aiFace face = mesh->mFaces[index];

		for (auto faceIndex{0} ; faceIndex < face.mNumIndices; ++faceIndex)
			indices.push_back(face.mIndices[faceIndex]);
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return Mesh(vertices, indices, textures);
}
