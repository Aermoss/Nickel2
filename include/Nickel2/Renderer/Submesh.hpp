#pragma once

#include <cstdint>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vertex.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Material.hpp"
#include "VertexArray.hpp"
#include "Buffer.hpp"

namespace Nickel2 {
    class Entity;
    
    class Submesh {
        private:
            Entity* entity;
            std::shared_ptr<VertexBuffer> vertexBuffer;
            std::shared_ptr<IndexBuffer> indexBuffer;
            std::shared_ptr<VertexArray> vertexArray;

        public:
            Material material;
            
            Submesh(Entity* entity, std::vector<Vertex> vertices, std::vector<uint32_t> indices, Material material);
            ~Submesh() {}
            
            void Render(std::shared_ptr<Shader> shader, bool useTexture = true);
            void Destroy();
    };
}