#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/Framebuffer.hpp>
#include <Nickel2/Renderer/RendererAPI.hpp>

#include <Nickel2/Renderer/OpenGL/OpenGLFramebuffer.hpp>

namespace Nickel2 {
    uint32_t Framebuffer::CalculateStride(uint32_t width, uint32_t channels) {
        uint32_t stride = width * channels;
        return stride + ((stride % 4) ? (4 - stride % 4) : 0);
    }

    std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPIType::None: return nullptr;
            case RendererAPIType::OpenGL: return std::make_shared<OpenGLFramebuffer>(spec);
            default: return nullptr;
        }
    }
}