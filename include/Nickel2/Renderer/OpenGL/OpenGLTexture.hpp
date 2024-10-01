#pragma once

#include <Nickel2/Renderer/Texture.hpp>

#include <glad/glad.h>

namespace Nickel2 {
    class OpenGLTexture2D : public Texture2D {
        private:
            TextureSpecification specification;

            std::string path;
            bool isLoaded = false, isHDR = false;
            uint32_t id, width, height;
            GLenum internalFormat, dataFormat;

        public:
            OpenGLTexture2D(const TextureSpecification& specification);
            OpenGLTexture2D(const std::string& path);
            virtual ~OpenGLTexture2D();

            virtual const TextureSpecification& GetSpecification() const override { return specification; }

            virtual uint32_t GetWidth() const override { return width;  }
            virtual uint32_t GetHeight() const override { return height; }
            virtual uint32_t GetRendererID() const override { return id; }

            virtual const std::string& GetPath() const override { return path; }
            
            virtual void SetData(void* data, uint32_t size) override;

            virtual void Bind(uint32_t binding = 0) const override;
            virtual void Unbind(uint32_t binding = 0) const override;

            virtual bool IsLoaded() const override { return isLoaded; }

            virtual bool operator==(const Texture& other) const override {
                return id == other.GetRendererID();
            }
    };
}