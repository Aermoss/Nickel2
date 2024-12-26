#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/OpenGL/OpenGLFramebuffer.hpp>

#include <glad/glad.h>

namespace Nickel2 {
    static const uint32_t maxFramebufferSize = 8192;

    namespace Utils {
        static GLenum TextureTarget(bool multisampled) {
            return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }

        static void CreateTextures(bool multisampled, uint32_t* id, uint32_t count) {
            glCreateTextures(TextureTarget(multisampled), count, id);
        }

        static void BindTexture(bool multisampled, uint32_t id) {
            glBindTexture(TextureTarget(multisampled), id);
        }

        static bool IsDepthFormat(FramebufferTextureFormat format) {
            switch (format) {
                case FramebufferTextureFormat::DEPTH24STENCIL8: return true;
                case FramebufferTextureFormat::DEPTH32FSTENCIL8UINT: return true;
                default: return false;
            }
        }

        static GLenum ToOpenGLTextureFormat(FramebufferTextureFormat format) {
            switch (format) {
                case FramebufferTextureFormat::RED8UN: return GL_RED;
                case FramebufferTextureFormat::RED8UI: return GL_RED_INTEGER;
                case FramebufferTextureFormat::RED16UI: return GL_RED_INTEGER;
                case FramebufferTextureFormat::RED32UI: return GL_RED_INTEGER;
                case FramebufferTextureFormat::RED32F: return GL_RED;
                case FramebufferTextureFormat::RG8: return GL_RG;
                case FramebufferTextureFormat::RG16F: return GL_RG;
                case FramebufferTextureFormat::RG32F: return GL_RG;
                case FramebufferTextureFormat::RGB: return GL_RGB;
                case FramebufferTextureFormat::SRGB: return GL_RGB;
                case FramebufferTextureFormat::RGBA: return GL_RGBA;
                case FramebufferTextureFormat::SRGBA: return GL_RGBA;
                case FramebufferTextureFormat::RGBA16F: return GL_RGBA;
                case FramebufferTextureFormat::RGBA32F: return GL_RGBA;
                case FramebufferTextureFormat::B10R11G11UF: return GL_RGB;
                default: return 0;
            }
        }

        static GLenum ToOpenGLInternalTextureFormat(FramebufferTextureFormat format) {
            switch (format) {
                case FramebufferTextureFormat::RED8UN: return GL_R8;
                case FramebufferTextureFormat::RED8UI: return GL_R8UI;
                case FramebufferTextureFormat::RED16UI: return GL_R16UI;
                case FramebufferTextureFormat::RED32UI: return GL_R32UI;
                case FramebufferTextureFormat::RED32F: return GL_R32F;
                case FramebufferTextureFormat::RG8: return GL_RG8;
                case FramebufferTextureFormat::RG16F: return GL_RG16F;
                case FramebufferTextureFormat::RG32F: return GL_RG32F;
                case FramebufferTextureFormat::RGB: return GL_RGB8;
                case FramebufferTextureFormat::SRGB: return GL_SRGB8;
                case FramebufferTextureFormat::RGBA: return GL_RGBA8;
                case FramebufferTextureFormat::SRGBA: return GL_SRGB8_ALPHA8;
                case FramebufferTextureFormat::RGBA16F: return GL_RGBA16F;
                case FramebufferTextureFormat::RGBA32F: return GL_RGBA32F;
                case FramebufferTextureFormat::B10R11G11UF: return GL_R11F_G11F_B10F;
                case FramebufferTextureFormat::DEPTH24STENCIL8: return GL_DEPTH24_STENCIL8;
                case FramebufferTextureFormat::DEPTH32FSTENCIL8UINT: return GL_DEPTH32F_STENCIL8;
                case FramebufferTextureFormat::DEPTH32F: return GL_DEPTH_COMPONENT32F;
                default: return 0;
            }
        }

        static GLenum ToOpenGLDataType(FramebufferTextureFormat format) {
            switch (format) {
                case FramebufferTextureFormat::RED8UN: return GL_UNSIGNED_BYTE;
                case FramebufferTextureFormat::RED8UI: return GL_UNSIGNED_BYTE;
                case FramebufferTextureFormat::RED16UI: return GL_UNSIGNED_BYTE;
                case FramebufferTextureFormat::RED32UI: return GL_UNSIGNED_BYTE;
                case FramebufferTextureFormat::RED32F: return GL_FLOAT;
                case FramebufferTextureFormat::RG8: return GL_UNSIGNED_BYTE;
                case FramebufferTextureFormat::RG16F: return GL_FLOAT;
                case FramebufferTextureFormat::RG32F: return GL_FLOAT;
                case FramebufferTextureFormat::RGB: return GL_UNSIGNED_BYTE;
                case FramebufferTextureFormat::SRGB: return GL_UNSIGNED_BYTE;
                case FramebufferTextureFormat::RGBA: return GL_UNSIGNED_BYTE;
                case FramebufferTextureFormat::SRGBA: return GL_UNSIGNED_BYTE;
                case FramebufferTextureFormat::RGBA16F: return GL_FLOAT;
                case FramebufferTextureFormat::RGBA32F: return GL_FLOAT;
                case FramebufferTextureFormat::B10R11G11UF: return GL_FLOAT;
                default: return 0;
            }
        }

        static void AttachColorTexture(uint32_t id, int samples, FramebufferTextureFormat format, uint32_t width, uint32_t height, int index) {
            bool multisampled = samples > 1;

            if (multisampled) {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, Utils::ToOpenGLInternalTextureFormat(format), width, height, GL_FALSE);
            } else {
                glTexImage2D(GL_TEXTURE_2D, 0, Utils::ToOpenGLInternalTextureFormat(format), width, height, 0, Utils::ToOpenGLTextureFormat(format), Utils::ToOpenGLDataType(format), nullptr);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            } glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
        }

        static void AttachDepthTexture(uint32_t id, int samples, FramebufferTextureFormat format, uint32_t width, uint32_t height) {
            bool multisampled = samples > 1;

            if (multisampled) {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, Utils::ToOpenGLInternalTextureFormat(format), width, height, GL_FALSE);
            } else {
                glTexStorage2D(GL_TEXTURE_2D, 1, Utils::ToOpenGLInternalTextureFormat(format), width, height);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            } glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, TextureTarget(multisampled), id, 0);
        }
    }

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& specification) : specification(specification) { Invalidate(); }
    OpenGLFramebuffer::~OpenGLFramebuffer() { Release(); }

    void OpenGLFramebuffer::Invalidate() {
        if (id != 0) Release();
        glCreateFramebuffers(1, &id);
        glBindFramebuffer(GL_FRAMEBUFFER, id);
        bool multisample = specification.samples > 1;

        for (FramebufferTextureFormat format : specification.attachments) {
            if (!Utils::IsDepthFormat(format)) {
                colorAttachmentFormats.push_back(format);
            } else {
                NK_CORE_ASSERT(depthAttachment == 0, "Framebuffer already has a depth attachment.");
                Utils::CreateTextures(multisample, &depthAttachment, 1);
                Utils::BindTexture(multisample, depthAttachment);
                Utils::AttachDepthTexture(depthAttachment, specification.samples, format, specification.width, specification.height);
            }
        }

        if (colorAttachmentFormats.size()) {
            colorAttachments.resize(colorAttachmentFormats.size());
            Utils::CreateTextures(multisample, colorAttachments.data(), colorAttachments.size());

            for (size_t i = 0; i < colorAttachments.size(); i++) {
                Utils::BindTexture(multisample, colorAttachments[i]);
                Utils::AttachColorTexture(colorAttachments[i], specification.samples, colorAttachmentFormats[i], specification.width, specification.height, i);
            }
        }

        if (colorAttachments.size() > 1) {
            NK_CORE_ASSERT(colorAttachments.size() <= 4);
            GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
            glDrawBuffers(colorAttachments.size(), buffers);
        } else if (colorAttachments.empty()) {
            glDrawBuffer(GL_NONE);
        }

        NK_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete.");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Release() {
        glDeleteFramebuffers(1, &id);
        glDeleteTextures(1, &depthAttachment);
        glDeleteTextures(colorAttachments.size(), colorAttachments.data());
        colorAttachments.clear(), colorAttachmentFormats.clear(), depthAttachment = 0;
    }

    void OpenGLFramebuffer::Bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, id);
        glViewport(0, 0, specification.width, specification.height);
    }

    void OpenGLFramebuffer::Unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height) {
        if (width == 0 || height == 0 || width > maxFramebufferSize || height > maxFramebufferSize) return;
        else { specification.width = width, specification.height = height; }
        Invalidate();
    }

    void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value) {
        NK_CORE_ASSERT(attachmentIndex < colorAttachments.size());
        glClearTexImage(colorAttachments[attachmentIndex], 0, Utils::ToOpenGLInternalTextureFormat(colorAttachmentFormats[attachmentIndex]), GL_INT, &value);
    }

    std::vector<uint8_t> OpenGLFramebuffer::GetPixels(std::vector<uint8_t>& buffer, uint32_t attachmentIndex, uint32_t channels) {
        NK_CORE_ASSERT(attachmentIndex < colorAttachments.size());
        NK_CORE_ASSERT(channels == 3 || channels == 4);
        buffer.resize(CalculateStride(specification.width, channels) * specification.height);
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        glReadPixels(0, 0, specification.width, specification.height, channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
        return buffer;
    }
}