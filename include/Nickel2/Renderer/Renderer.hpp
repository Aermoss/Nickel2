#pragma once

#include <Nickel2/Renderer/Shader.hpp>
#include <Nickel2/Renderer/RenderCommand.hpp>

namespace Nickel2 {
    class Shader;
    
    class Renderer {
        public:
            static void Initialize();
            static void Terminate();
            
            static void OnWindowResize(uint32_t width, uint32_t height);

            static void BeginScene();
            static void EndScene();

            static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
    };
}