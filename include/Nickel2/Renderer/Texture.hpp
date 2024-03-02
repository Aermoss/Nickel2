#pragma once

#include <cstdint>
#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <stb/stb_image.h>

#include "../Core/Constants.hpp"

#include "Shader.hpp"

namespace Nickel2 {
    extern const char* textureTypes[6];
    extern const char* textureTypesUniformNames[6];
    
    struct TextureConfig {
        int32_t minFilter = NICKEL2_LINEAR_MIPMAP_LINEAR, magFilter = NICKEL2_LINEAR;
        int32_t wrapS = NICKEL2_REPEAT, wrapT = NICKEL2_REPEAT;
        int32_t internalFormat = NICKEL2_RGBA, format = NICKEL2_NULL;
        int32_t pixelType = NICKEL2_UNSIGNED_BYTE, mipmap = NICKEL2_TRUE;
        int32_t flipVertically = NICKEL2_TRUE;
    };

    class Shader;
    
    class Texture {
        private:
            std::string filePath;
            uint32_t id, slot;

        public:
            int32_t width, height, channels, format;

            Texture(std::string filePath, uint32_t slot, TextureConfig config = {});
            ~Texture();
            
            std::string& getFilePath();
            void texUnit(Shader* shader, const char* name);
            void bind();
            void unbind();
            void destroy();
    };
}