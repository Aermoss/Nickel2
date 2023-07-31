#include "texture.hpp"

namespace nickel2 {
    const char* textureTypes[] = {
        "albedoMap", "roughnessMap", "metallicMap", "normalMap", "specularMap", "ambientMap"
    };

    const char* textureTypesUniformNames[] = {
        "useAlbedoMap", "useRoughnessMap", "useMetallicMap", "useNormalMap", "useSpecularMap", "useAmbientMap"
    };
    
    Texture::Texture(std::string filePath, uint32_t slot, TextureConfig config) : filePath(filePath), slot(slot) {
        glGenTextures(1, &id);
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.magFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.wrapT);

        int32_t width, height, channels, format = config.format;
        stbi_set_flip_vertically_on_load(true);

        if (config.pixelType == GL_UNSIGNED_BYTE) {
            unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

            if (format == 0)
                format = channels == 4 ? GL_RGBA : (channels == 3 ? GL_RGB : (channels == 2 ? GL_RG : (channels == 1 ? GL_RED : 0)));

            if (format == 0 || data == nullptr) {
                context->logger->log(NICKEL2_ERROR, ("failed to load image: \"" + filePath + "\".").c_str());
                return;
            } else {
                context->logger->log(NICKEL2_INFO, ("image successfully loaded: \"" + filePath + "\".").c_str());
            }

            glTexImage2D(GL_TEXTURE_2D, 0, config.internalFormat, width, height, 0, format, config.pixelType, data);
            if (config.mipmap) glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
            stbi_image_free(data);
        } else if (config.pixelType == GL_FLOAT) {
            float* data = stbi_loadf(filePath.c_str(), &width, &height, &channels, 0);

            if (format == 0)
                format = channels == 4 ? GL_RGBA : (channels == 3 ? GL_RGB : (channels == 2 ? GL_RG : (channels == 1 ? GL_RED : 0)));

            if (format == 0 || data == nullptr) {
                context->logger->log(NICKEL2_ERROR, ("failed to load image: \"" + filePath + "\".").c_str());
                return;
            } else {
                context->logger->log(NICKEL2_INFO, ("image successfully loaded: \"" + filePath + "\".").c_str());
            }

            glTexImage2D(GL_TEXTURE_2D, 0, config.internalFormat, width, height, 0, format, config.pixelType, data);
            if (config.mipmap) glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
            stbi_image_free(data);
        } else {
            context->logger->log(NICKEL2_ERROR, ("failed to load image, unknown pixel type: \"" + filePath + "\"").c_str());
        }
    }

    Texture::~Texture() {

    }

    std::string& Texture::getFilePath() {
        return filePath;
    }

    void Texture::texUnit(Shader* shader, const char* name) {
        shader->setUniform1i(name, slot);
    }

    void Texture::bind() {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void Texture::unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::destroy() {
        glDeleteTextures(1, &id);
    }
}