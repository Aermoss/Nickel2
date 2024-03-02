#pragma once

#include <cstdint>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Scene/Transform.hpp"

#include "Vertex.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Material.hpp"
#include "VertexArray.hpp"
#include "Buffer.hpp"

namespace Nickel2 {
    class SubMesh {
        private:
            std::vector <Vertex> vertices;
            std::vector <uint32_t> indices;
            Buffer* vertexBuffer;
            Buffer* indexBuffer;
            VertexArray* vertexArray;

            void setup();

        public:
            Material material;
            Transform* transform;
            
            SubMesh(std::vector <Vertex> vertices, std::vector <uint32_t> indices, Material& material, Transform* parent = nullptr);
            ~SubMesh();

            std::vector <Vertex> getVertices();
            std::vector <uint32_t> getIndices();
            
            void render(Shader* shader, bool useTexture = true);
            void destroy();
    };
}