#pragma once

#include <stdint.h>

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "shader.hpp"

namespace nickel2 {
    extern const char* textureTypes[6];
    
    struct TextureConfig {
        int32_t minFilter, magFilter;
        int32_t wrapS, wrapT;
        int32_t internalFormat, format;
        int32_t pixelType, mipmap;
    };

    class Texture {
        private:
            uint32_t id, slot;
            const char* filePath;

        public:
            Texture(const char* filePath, uint32_t slot, TextureConfig config = \
                         {GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_RGBA, 0, GL_UNSIGNED_BYTE, GL_TRUE});
            ~Texture();
            
            void texUnit(Shader* shader, const char* name);
            void bind();
            void unbind();
            void destroy();
    };
}