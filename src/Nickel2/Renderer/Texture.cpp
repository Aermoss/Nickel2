#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/Texture.hpp>
#include <Nickel2/Renderer/Renderer.hpp>

#include <Nickel2/Renderer/OpenGL/OpenGLTexture.hpp>

namespace Nickel2 {
    std::shared_ptr<Texture2D> Texture2D::Create(const TextureSpecification& specification) {
        switch (Renderer::GetAPI()) {
            case API::None: return nullptr;
            case API::OpenGL: return std::make_shared<OpenGLTexture2D>(specification);
        } return nullptr;
    }

    std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path) {
        switch (Renderer::GetAPI()) {
            case API::None: return nullptr;
            case API::OpenGL: return std::make_shared<OpenGLTexture2D>(path);
        } return nullptr;
    }
}