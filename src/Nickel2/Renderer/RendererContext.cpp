#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/RendererContext.hpp>
#include <Nickel2/Renderer/Renderer.hpp>

#include <Nickel2/Renderer/OpenGL/OpenGLContext.hpp>

namespace Nickel2 {
    std::unique_ptr<RendererContext> RendererContext::Create(void* window) {
        switch (Renderer::GetAPI()) {
            case API::None: return nullptr;
            case API::OpenGL: return std::make_unique<OpenGLContext>(static_cast<GLFWwindow*>(window));
        } return nullptr;
    }
}