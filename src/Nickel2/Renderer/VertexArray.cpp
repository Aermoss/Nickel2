#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/VertexArray.hpp>
#include <Nickel2/Renderer/Renderer.hpp>

#include <Nickel2/Renderer/OpenGL/OpenGLVertexArray.hpp>

namespace Nickel2 {
    std::shared_ptr<VertexArray> VertexArray::Create() {
        switch (Renderer::GetAPI()) {
            case API::None: return nullptr;
            case API::OpenGL: return std::make_shared<OpenGLVertexArray>();
            default: break;
        } return nullptr;
    }
}