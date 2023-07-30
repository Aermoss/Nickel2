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
#include "material.hpp"
#include "mesh.hpp"

namespace nickel2 {
    class Model {
        private:
            std::vector <Mesh> meshes;
            std::vector <Texture*> loadedTextures;
            std::string directory;

            void loadModel(std::string const& path);
            void processNode(aiNode* node, const aiScene* scene);
            Mesh processMesh(aiMesh* mesh, const aiScene* scene);
            Texture* getMaterialTexture(aiMaterial* mat, aiTextureType type, uint32_t slot);

        public:
            Transform* transform;
            
            Model(std::string const& path);
            ~Model();

            void render(Shader* shader, bool useTexture = true);
            void destroy();
    };
}