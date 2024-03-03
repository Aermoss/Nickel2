#pragma once

#include <string>
#include <cstdint>
#include <unordered_map>
#include <dwmapi.h>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE
#include <glfw/glfw3native.h>
#include <stb/stb_image.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

#include "Context.hpp"
#include "Color.hpp"
#include "Input.hpp"

namespace Nickel2 {
    typedef void (*windowEvent_t)();

    class Window {
        private:
            GLFWwindow* window;
            ImGuiContext* imguiContext;
            std::unordered_map<std::string, windowEvent_t> events;
            std::string title;
            uint32_t id;

        public:
            InputHandler* input;
            int32_t width, height;
            Color backgroundColor;
            float deltaTime, currentTime, lastTime;
            
            Window(int32_t width, int32_t height, const char* title, const Color& backgroundColor = {0.0f, 0.0f, 0.0f, 255.0f}, bool vsync = false, bool fullscreen = false, bool icon = true, bool darkMode = true);
            ~Window();

            void update();
            void clear();
            void setEvent(std::string name, windowEvent_t func);
            void setVSync(bool vsync);
            void setTitle(const char* title);
            void getSize(int32_t* width, int32_t* height);
            HWND getWindowHandle();
            GLFWwindow* getGLFWWindow();
            ImGuiContext* getImGuiContext();
            std::string getTitle();
            bool shouldClose();
            void swapBuffers();
            void destroy();
    };
}