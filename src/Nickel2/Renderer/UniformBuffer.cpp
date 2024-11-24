#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/UniformBuffer.hpp>
#include <Nickel2/Renderer/RendererAPI.hpp>

#include <Nickel2/Renderer/OpenGL/OpenGLUniformBuffer.hpp>

namespace Nickel2 {
    std::shared_ptr<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPIType::OpenGL: return std::make_shared<OpenGLUniformBuffer>(size, binding);
            default: return nullptr;
        }
    }
}