#pragma once

#include <stdint.h>
#include <vector>

#include "vertex.hpp"
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

            void setupMesh();

        public:
            Mesh(std::vector <Vertex> vertices, std::vector <uint32_t> indices, Material& material);
            ~Mesh();

            void render(Shader* shader);
            void destroy();
    };
}