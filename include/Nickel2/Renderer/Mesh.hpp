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

#include "Submesh.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"
#include "Material.hpp"

namespace Nickel2 {
    class Entity;
    class TextureManager;
    class Submesh;
    
    class Mesh {
        private:
            Entity* entity;
            std::unordered_map<std::string, std::shared_ptr<Entity>> submeshes;
            std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
            std::string directory;

            void LoadMesh(std::string const& path);
            void ProcessNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform = glm::mat4(1.0f));
            void ProcessMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& parentTransform = glm::mat4(1.0f));

        public:
            Mesh(Entity* entity, std::string const& path);
            ~Mesh() { this->Destroy(); }

            std::unordered_map<std::string, std::shared_ptr<Entity>>& GetSubmeshes() { return submeshes; }
            std::unordered_map<std::string, std::shared_ptr<Texture>>& GetTextures() { return textures; }
            const std::string& GetDirectory() { return directory; }

            void Render(std::shared_ptr<Shader> shader, bool useTexture = true, bool skipTransparent = false);
            void Destroy();
    };
}