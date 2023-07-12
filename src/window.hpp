#pragma once

#include <stdint.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "input.hpp"

namespace nickel2 {
    class Window {
        private:
            GLFWwindow* window;

        public:
            InputHandler* input;
            float deltaTime, currentTime, lastTime;
            
            Window(uint32_t width, uint32_t height, const char* title, bool vsync = false);
            ~Window();

            void update();
            void clear();
            void getSize(int32_t* width, int32_t* height);
            bool shouldClose();
            void swapBuffers();
            void destroy();
    };
}