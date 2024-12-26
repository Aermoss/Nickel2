#pragma once

#include <Nickel2/Core/Base.hpp>

#include <vector>

namespace Nickel2 {
    enum class FramebufferTextureFormat {
        None = 0, RED8UN, RED8UI, RED16UI, RED32UI, RED32F, RG8, RG16F, RG32F, RGB, RGBA, RGBA16F, RGBA32F, \
        B10R11G11UF, SRGB, SRGBA, DEPTH32FSTENCIL8UINT, DEPTH32F, DEPTH24STENCIL8, Depth = DEPTH24STENCIL8,
    };

    struct FramebufferSpecification {
        uint32_t width, height, samples = 1;
        std::vector<FramebufferTextureFormat> attachments;
    };

    class Framebuffer {
        public:
            virtual ~Framebuffer() = default;

            virtual void Bind() = 0;
            virtual void Unbind() = 0;

            virtual void Resize(uint32_t width, uint32_t height) = 0;
            virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

            virtual uint32_t GetDepthAttachment() const = 0;
            virtual uint32_t GetColorAttachment(uint32_t index = 0) const = 0;
            virtual std::vector<uint8_t> GetPixels(std::vector<uint8_t>& buffer, uint32_t attachmentIndex = 0, uint32_t channels = 3) = 0;
            virtual const FramebufferSpecification& GetSpecification() const = 0;

            static uint32_t CalculateStride(uint32_t width, uint32_t channels);
            static std::shared_ptr<Framebuffer> Create(const FramebufferSpecification& specification);
    };
}