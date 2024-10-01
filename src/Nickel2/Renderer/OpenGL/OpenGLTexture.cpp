#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/OpenGL/OpenGLTexture.hpp>

#include <stb/stb_image.h>

namespace Nickel2 {
    namespace Utils {
        static GLenum RoentgenImageFormatToGLDataFormat(ImageFormat format) {
            switch (format) {
                case ImageFormat::RGB8: return GL_RGB;
                case ImageFormat::RGBA8: return GL_RGBA;
                default: break;
            } return 0;
        }
        
        static GLenum RoentgenImageFormatToGLInternalFormat(ImageFormat format) {
            switch (format) {
                case ImageFormat::RGB8: return GL_RGB8;
                case ImageFormat::RGBA8: return GL_RGBA8;
                default: break;
            } return 0;
        }
    }

    OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification)
        : specification(specification), width(specification.width), height(specification.height),
          internalFormat(Utils::RoentgenImageFormatToGLInternalFormat(specification.format)),
          dataFormat(Utils::RoentgenImageFormatToGLDataFormat(specification.format)) {

        glCreateTextures(GL_TEXTURE_2D, 1, &id);
        glTextureStorage2D(id, 1, internalFormat, width, height);

        glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : path(path) {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        void* data = nullptr;

        if (stbi_is_hdr(path.c_str())) {
            data = (void*) stbi_loadf(path.c_str(), &width, &height, &channels, 0);
            this->isHDR = true;
        } else {
            data = (void*) stbi_load(path.c_str(), &width, &height, &channels, 0);
        }
            
        if (data != nullptr) {
            this->isLoaded = true;
            this->width = width, this->height = height;
            GLenum internalFormat = 0, dataFormat = 0;

            if (channels == 4)
                internalFormat = this->isHDR ? GL_RGBA16F : GL_RGBA8, dataFormat = GL_RGBA;

            if (channels == 3)
                internalFormat = this->isHDR ? GL_RGB16F : GL_RGB8, dataFormat = GL_RGB;

            this->internalFormat = internalFormat;
            this->dataFormat = dataFormat;

            NK_CORE_ASSERT(internalFormat & dataFormat, "Unsupported format.");

            glCreateTextures(GL_TEXTURE_2D, 1, &id);
            glTextureStorage2D(id, 1, internalFormat, width, height);
            glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTextureSubImage2D(id, 0, 0, 0, width, height, dataFormat, this->isHDR ? GL_FLOAT : GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);

            Nickel2::Logger::Log(Nickel2::Logger::Level::Info, "Texture2D", ("Texture loaded successfully: \"" + path + "\", id: " + std::to_string(id) + ".").c_str());
        } else {
            Nickel2::Logger::Log(Nickel2::Logger::Level::Info, "Texture2D", ("Failed to load texture: \"" + path + "\".").c_str());
        }
    }

    OpenGLTexture2D::~OpenGLTexture2D() {
        Nickel2::Logger::Log(Nickel2::Logger::Level::Info, "Texture2D", ("Texture destroyed successfully: \"" + path + "\", id: " + std::to_string(id) + ".").c_str());
        glDeleteTextures(1, &id);
    }

    void OpenGLTexture2D::SetData(void* data, uint32_t size) {
        uint32_t bpp = dataFormat == GL_RGBA ? 4 : 3;
        NK_CORE_ASSERT(size == width * height * bpp, "Incomplete data.");
        glTextureSubImage2D(id, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
    }

    void OpenGLTexture2D::Bind(uint32_t binding) const {
        glBindTextureUnit(binding, id);
    }

    void OpenGLTexture2D::Unbind(uint32_t binding) const {
        glBindTextureUnit(binding, 0);
    }
}