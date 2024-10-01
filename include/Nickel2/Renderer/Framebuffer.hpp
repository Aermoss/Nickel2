#pragma once

#include <Nickel2/Core/Base.hpp>

#include <vector>

namespace Nickel2 {
    enum class FramebufferTextureFormat {
        None = 0, RGBA8, RED_INTEGER, DEPTH24STENCIL8, Depth = DEPTH24STENCIL8
    };

    struct FramebufferTextureSpecification {
        FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;

        FramebufferTextureSpecification() = default;
        FramebufferTextureSpecification(FramebufferTextureFormat format) : textureFormat(format) {}
    };

    struct FramebufferAttachmentSpecification {
        std::vector<FramebufferTextureSpecification> attachments;

        FramebufferAttachmentSpecification() = default;
        FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments) : attachments(attachments) {}
    };

    struct FramebufferSpecification {
        uint32_t width = 0, height = 0, samples = 1;
        FramebufferAttachmentSpecification attachments;
    };

    class Framebuffer {
        public:
            virtual ~Framebuffer() = default;

            virtual void Bind() = 0;
            virtual void Unbind() = 0;

            virtual void Resize(uint32_t width, uint32_t height) = 0;
            virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

            virtual std::vector<uint8_t> GetPixels(std::vector<uint8_t>& buffer, uint32_t attachmentIndex = 0, uint32_t channels = 3) = 0;
            virtual uint32_t GetColorAttachmentId(uint32_t index = 0) const = 0;
            virtual const FramebufferSpecification& GetSpecification() const = 0;

            static uint32_t CalculateStride(uint32_t width, uint32_t channels);
            static std::shared_ptr<Framebuffer> Create(const FramebufferSpecification& spec);
    };
}