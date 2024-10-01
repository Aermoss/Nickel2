#pragma once

#include <Nickel2/Renderer/RendererAPI.hpp>

namespace Nickel2 {
    class RenderCommand {
        private:
            static std::unique_ptr<RendererAPI> rendererAPI;
            
        public:
            static void Initialize() {
                rendererAPI->Initialize();
            }

            static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
                rendererAPI->SetViewport(x, y, width, height);
            }

            static void SetClearColor(const glm::vec4& color) {
                rendererAPI->SetClearColor(color);
            }

            static void Clear() {
                rendererAPI->Clear();
            }

            static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) {
                rendererAPI->DrawIndexed(vertexArray, indexCount);
            }

            static void DrawLines(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount) {
                rendererAPI->DrawLines(vertexArray, vertexCount);
            }

            static void SetLineWidth(float width) {
                rendererAPI->SetLineWidth(width);
            }
    };
}