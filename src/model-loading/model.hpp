#ifndef MODEL_HPP
#define MODEL_HPP

#include "shader.hpp"
#include "mesh.hpp"
#include "texture.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

class Model {
	public:
		Model(std::string &path);
		void Draw(Shader &shader);
	private:
		std::vector<Mesh> meshes;
		std::string directory;
		std::vector<Texture> textures_loaded;

		void loadModel(std::string &path);
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial *mat, const aiTextureType &type, const std::string &typeName);
};
#endif
