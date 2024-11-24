#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/VertexArray.hpp>
#include <Nickel2/Renderer/RendererAPI.hpp>

#include <Nickel2/Renderer/OpenGL/OpenGLVertexArray.hpp>

namespace Nickel2 {
    std::shared_ptr<VertexArray> VertexArray::Create() {
        switch (RendererAPI::GetAPI()) {
            case RendererAPIType::OpenGL: return std::make_shared<OpenGLVertexArray>();
            default: return nullptr;
        }
    }
}