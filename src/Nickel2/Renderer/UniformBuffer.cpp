#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/UniformBuffer.hpp>
#include <Nickel2/Renderer/Renderer.hpp>

#include <Nickel2/Renderer/OpenGL/OpenGLUniformBuffer.hpp>

namespace Nickel2 {
    std::shared_ptr<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding) {
        switch (Renderer::GetAPI()) {
            case API::None: return nullptr;
            case API::OpenGL: return std::make_shared<OpenGLUniformBuffer>(size, binding);
            default: break;
        } return nullptr;
    }
}