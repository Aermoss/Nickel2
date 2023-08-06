#pragma once

#include <stdint.h>

#include <glad/glad.h>

namespace nickel2 {
    class Buffer {
        private:
            uint32_t id, type;

        public:
            Buffer(uint32_t type);
            ~Buffer();

            void bind();
            void unbind();
            void bufferData(uint32_t size, const void* data, uint32_t usage = GL_STATIC_DRAW);
            void destroy();
    };
}