#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/RendererAPI.hpp>
#include <Nickel2/Renderer/Renderer.hpp>

#include <Nickel2/Renderer/OpenGL/OpenGLRendererAPI.hpp>

namespace Nickel2 {
    API RendererAPI::api = API::OpenGL;

    Scope<RendererAPI> RendererAPI::Create() {
        switch (Renderer::GetAPI()) {
            case API::None: return nullptr;
            case API::OpenGL: return std::make_unique<OpenGLRendererAPI>();
        } return nullptr;
    }
}