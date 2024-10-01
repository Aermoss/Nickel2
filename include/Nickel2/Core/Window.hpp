#pragma once

#include "Base.hpp"
#include "../Event/Event.hpp"
#include "../Renderer/RendererContext.hpp"

#include <functional>
#include <filesystem>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <glfw/glfw3native.h>

#include <glm/glm.hpp>

namespace Nickel2 {
    struct WindowSpecification {
        std::string title = "Nickel2";
        std::filesystem::path iconPath;
        int32_t width = 1600, height = 900, x = -1, y = -1;
        bool decorated = true, fullscreen = false, maximized = false;
        bool darkMode = true, vSync = true, resizable = true;
    };

    class Window {
        using EventCallbackFunc = std::function<void(Event&)>;

        private:
            GLFWwindow* window = nullptr;
            WindowSpecification specification;
            EventCallbackFunc eventCallback;

            std::unique_ptr<RendererContext> rendererContext;
            bool isTerminated = false;

        public:
            Window(const WindowSpecification& specification) : specification(specification) {}
            ~Window() {}

            void Initialize();
            void Terminate();

            void ProcessEvents();
            void SwapBuffers();

            void Maximize();
            void Center();

            void SetSize(const glm::vec2& size);
            glm::vec2 GetSize();

            void SetPosition(const glm::vec2& position);
            glm::vec2 GetPosition();

            void SetTitle(const std::string& title);
            const std::string GetTitle();

            void SetEventCallback(const EventCallbackFunc& callback);
            EventCallbackFunc GetEventCallback();

            void SetVSync(bool enabled);
            bool GetVSync();

            inline void* GetHandle() const { return window; }
            inline HWND GetHWND() const { return glfwGetWin32Window(window); }
            inline float GetTime() const { return (float) glfwGetTime(); }

            static Window* Create(const WindowSpecification& specification = WindowSpecification());
    };
}