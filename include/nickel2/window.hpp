#pragma once

#include <stdint.h>
#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "context.hpp"
#include "color.hpp"
#include "input.hpp"

namespace nickel2 {
    typedef void (*windowEvent_t)();

    class Window {
        private:
            GLFWwindow* window;
            std::unordered_map <std::string, windowEvent_t> events;
            std::string title;
            uint32_t id;

        public:
            InputHandler* input;
            int32_t width, height;
            Color backgroundColor;
            float deltaTime, currentTime, lastTime;
            
            Window(int32_t width, int32_t height, const char* title, const Color& backgroundColor = {0.0f, 0.0f, 0.0f, 255.0f}, bool vsync = false, bool fullscreen = false, bool icon = true);
            ~Window();

            void update();
            void clear();
            void setEvent(std::string name, windowEvent_t func);
            void setVSync(bool vsync);
            void setTitle(const char* title);
            void getSize(int32_t* width, int32_t* height);
            GLFWwindow* getGLFWWindow();
            std::string getTitle();
            bool shouldClose();
            void swapBuffers();
            void destroy();
    };
}