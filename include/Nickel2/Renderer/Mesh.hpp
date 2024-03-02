#pragma once

#include <cstdint>
#include <unordered_map>
#include <future>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "../Scene/Transform.hpp"

#include "Submesh.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"
#include "Material.hpp"

namespace Nickel2 {
    class SubMesh;
    
    class Mesh {
        private:
            std::unordered_map<std::string, SubMesh*> subMeshes;
            std::unordered_map<std::string, Texture*> loadedTextures;
            // std::vector<std::future<void>> futures;
            // std::mutex materialMutex;
            // std::mutex textureMutex;
            // std::mutex subMeshMutex;
            std::string directory;

            void loadMesh(std::string const& path);
            void processNode(aiNode* node, const aiScene* scene);
            void processMesh(aiMesh* mesh, const aiScene* scene);
            Texture* getMaterialTexture(aiMaterial* mat, aiTextureType type, uint32_t slot);

        public:
            Transform* transform;
            
            Mesh(Transform* transform, std::string const& path);
            ~Mesh();

            std::unordered_map <std::string, SubMesh*>& getSubMeshes();
            std::unordered_map <std::string, Texture*>& getTextures();
            const std::string& getDirectory();

            void render(Shader* shader, bool useTexture = true, bool skipTransparent = false);
            void destroy();
    };
}