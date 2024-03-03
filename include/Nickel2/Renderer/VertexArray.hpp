#pragma once

#include <cstdint>

#include <glad/glad.h>

namespace Nickel2 {
    class VertexArray {
        private:
            uint32_t id;

        public:
            VertexArray();
            ~VertexArray();

            void bind();
            void unbind();
            void defineAttrib(uint32_t index, uint32_t size, uint32_t stride, const void* pointer, uint32_t type = GL_FLOAT, bool normalized = false);
            void destroy();
    };
}