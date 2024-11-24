#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/RendererAPI.hpp>
#include <Nickel2/Renderer/RendererAPI.hpp>

#include <Nickel2/Renderer/OpenGL/OpenGLAPI.hpp>

namespace Nickel2 {
    std::unique_ptr<RendererAPI> RendererAPI::Create() {
        switch (RendererAPI::GetAPI()) {
            case RendererAPIType::OpenGL: return std::make_unique<OpenGLRendererAPI>();
            default: return nullptr;
        }
    }
}