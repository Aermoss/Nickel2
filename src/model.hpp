#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "texture.hpp"
#include "shader.hpp"
#include "vertex.hpp"
#include "mesh.hpp"

namespace nickel2 {
    class Model {
        private:
            std::vector <Mesh> meshes;
            std::string directory;

            void loadModel(std::string const& path);
            void processNode(aiNode* node, const aiScene* scene);
            Mesh processMesh(aiMesh* mesh, const aiScene* scene);
            std::vector <Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type);

        public:
            Model(std::string const& path);
            ~Model();

            void render(Shader* shader);
            void destroy();
    };
}