#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/RendererContext.hpp>
#include <Nickel2/Renderer/RendererAPI.hpp>

#include <Nickel2/Renderer/OpenGL/OpenGLContext.hpp>

namespace Nickel2 {
    std::unique_ptr<RendererContext> RendererContext::Create(void* window) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPIType::OpenGL: return std::make_unique<OpenGLContext>(static_cast<GLFWwindow*>(window));
            default: return nullptr;
        }
    }
}