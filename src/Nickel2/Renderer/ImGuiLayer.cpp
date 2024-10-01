#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/ImGuiLayer.hpp>
#include <Nickel2/Renderer/Renderer.hpp>

#include <Nickel2/Renderer/OpenGL/OpenGLImGuiLayer.hpp>

namespace Nickel2 {
    ImGuiLayer* ImGuiLayer::Create() {
        switch (RendererAPI::GetAPI()) {
            case API::OpenGL: return new OpenGLImGuiLayer();
			default: return nullptr;
        }
    }
}