#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/Texture.hpp>
#include <Nickel2/Renderer/RendererAPI.hpp>

#include <Nickel2/Renderer/OpenGL/OpenGLTexture.hpp>

namespace Nickel2 {
    std::shared_ptr<Texture2D> Texture2D::Create(const TextureSpecification& specification) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPIType::OpenGL: return std::make_shared<OpenGLTexture2D>(specification);
            default: return nullptr;
        }
    }

    std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPIType::OpenGL: return std::make_shared<OpenGLTexture2D>(path);
            default: return nullptr;
        }
    }
}