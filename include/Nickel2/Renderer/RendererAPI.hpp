#pragma once

#include <Nickel2/Renderer/VertexArray.hpp>

#include <glm/glm.hpp>

namespace Nickel2 {
    enum class RendererAPIType {
        None = 0, OpenGL
    };

    class RendererAPI {
        private:
            inline static RendererAPIType api = RendererAPIType::OpenGL;

        public:
            virtual ~RendererAPI() = default;

            virtual void Initialize() = 0;
            virtual void Terminate() = 0;

            virtual void SetLineWidth(float width) = 0;
            virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
            virtual void SetClearColor(const glm::vec4& color) = 0;
            virtual void Clear() = 0;

            virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
            virtual void DrawLines(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount) = 0;

            static RendererAPIType GetAPI() { return api; }
            static std::unique_ptr<RendererAPI> Create();
    };
}