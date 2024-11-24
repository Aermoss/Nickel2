#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/Buffer.hpp>
#include <Nickel2/Renderer/RendererAPI.hpp>

#include <Nickel2/Renderer/OpenGL/OpenGLBuffer.hpp>

namespace Nickel2 {
    std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPIType::OpenGL: return std::make_shared<OpenGLVertexBuffer>(size);
            default: return nullptr;
        }
    }

    std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPIType::OpenGL: return std::make_shared<OpenGLVertexBuffer>(vertices, size);
            default: return nullptr;
        }
    }

    std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPIType::OpenGL: return std::make_shared<OpenGLIndexBuffer>(indices, size);
            default: return nullptr;
        }
    }
}