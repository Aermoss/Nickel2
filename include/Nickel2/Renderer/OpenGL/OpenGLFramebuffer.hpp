#pragma once

#include <Nickel2/Renderer/Framebuffer.hpp>

namespace Nickel2 {
    class OpenGLFramebuffer : public Framebuffer {
        private:
            uint32_t id = 0;
            FramebufferSpecification specification;

            std::vector<FramebufferTextureSpecification> colorAttachmentSpecifications;
            FramebufferTextureSpecification depthAttachmentSpecification = FramebufferTextureFormat::None;

            std::vector<uint32_t> colorAttachments;
            uint32_t depthAttachment = 0;

        public:
            OpenGLFramebuffer(const FramebufferSpecification& spec);
            virtual ~OpenGLFramebuffer();

            void Invalidate();

            virtual void Bind() override;
            virtual void Unbind() override;

            virtual void Resize(uint32_t width, uint32_t height) override;
            virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

            virtual std::vector<uint8_t> GetPixels(std::vector<uint8_t>& buffer, uint32_t attachmentIndex = 0, uint32_t channels = 3) override;
            virtual uint32_t GetColorAttachmentId(uint32_t index = 0) const override { NK_CORE_ASSERT(index < colorAttachments.size()); return colorAttachments[index]; }
            virtual const FramebufferSpecification& GetSpecification() const override { return specification; }
    };
}