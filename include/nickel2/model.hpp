#pragma once

#include <stdint.h>
#include <unordered_map>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "vertex.hpp"
#include "material.hpp"

namespace nickel2 {
    class Transform;
    class Shader;
    class Mesh;
    
    class Model {
        private:
            std::unordered_map <std::string, Mesh*> meshes;
            std::unordered_map <std::string, Texture*> loadedTextures;
            std::string directory;

            void loadModel(std::string const& path);
            void processNode(aiNode* node, const aiScene* scene);
            void processMesh(aiMesh* mesh, const aiScene* scene);
            Texture* getMaterialTexture(aiMaterial* mat, aiTextureType type, uint32_t slot);

        public:
            Transform* transform;
            
            Model(std::string const& path);
            ~Model();

            std::unordered_map <std::string, Mesh*>& getMeshes();
            std::unordered_map <std::string, Texture*>& getTextures();
            const std::string& getDirectory();

            void render(Shader* shader, bool useTexture = true);
            void destroy();
    };
}