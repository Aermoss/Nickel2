#pragma once

#include <Nickel2/Renderer/RendererAPI.hpp>

namespace Nickel2 {
    class OpenGLRendererAPI : public RendererAPI {
        public:
            virtual void Initialize() override;
            virtual void Terminate() override;

            virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
            virtual void SetClearColor(const glm::vec4& color) override;
            
            virtual void Clear() override;

            virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
            virtual void DrawLines(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount) override;
            
            virtual void SetLineWidth(float width) override;
    };
}