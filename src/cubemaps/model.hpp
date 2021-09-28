#ifndef MODEL_HPP
#define MODEL_HPP

#include "shader.hpp"
#include "mesh.hpp"

#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

std::uint32_t TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

class Model {
	public:
		Model(const std::string &path);
		void Draw(Shader &shader);
		void Draw();
	private:
		std::vector<Mesh> meshes;
		std::string directory;
		std::vector<ITexture> textures_loaded;

		void loadModel(const std::string &path);
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		std::vector<ITexture> loadMaterialTextures(aiMaterial *mat, const aiTextureType &type, const std::string &typeName);
};
#endif
