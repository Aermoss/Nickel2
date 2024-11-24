#pragma once

#include <Nickel2/Renderer/RendererAPI.hpp>

namespace Nickel2 {
    class RenderCommand {
        private:
            static std::unique_ptr<RendererAPI> api;
            
        public:
            static void Initialize() { api->Initialize(); }
            static void Terminate() { api->Terminate(); }

            static void SetLineWidth(float width) { api->SetLineWidth(width); }
            static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { api->SetViewport(x, y, width, height); }
            static void SetClearColor(const glm::vec4& color) { api->SetClearColor(color); }
            static void Clear() { api->Clear(); }

            static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) { api->DrawIndexed(vertexArray, indexCount); }
            static void DrawLines(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount) { api->DrawLines(vertexArray, vertexCount); }

            static RendererAPI* GetAPI() { return api.get(); }
    };
}