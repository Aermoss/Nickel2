#pragma once

#include <stdint.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "vertex.hpp"
#include "transform.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "material.hpp"
#include "vertexArray.hpp"
#include "buffer.hpp"

namespace nickel2 {
    class Mesh {
        private:
            std::vector <Vertex> vertices;
            std::vector <uint32_t> indices;
            Material material;
            Buffer* vertexBuffer;
            Buffer* indexBuffer;
            VertexArray* vertexArray;
            bool destroyTransform;

            void setupMesh();

        public:
            Transform* transform;
            
            Mesh(std::vector <Vertex> vertices, std::vector <uint32_t> indices, Material& material, Transform* transform = nullptr);
            ~Mesh();

            void render(Shader* shader);
            void destroy();
    };
}