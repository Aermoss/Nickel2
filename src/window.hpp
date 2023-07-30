#pragma once

#include <stdint.h>
#include <string>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "context.hpp"
#include "input.hpp"

namespace nickel2 {
    typedef void (*windowEvent_t)();

    class Window {
        private:
            GLFWwindow* window;
            std::map <std::string, windowEvent_t> events;
            uint32_t id;

        public:
            InputHandler* input;
            float deltaTime, currentTime, lastTime;
            
            Window(uint32_t width, uint32_t height, const char* title, bool vsync = false);
            ~Window();

            void update();
            void clear();
            void setEvent(std::string name, windowEvent_t func);
            void getSize(int32_t* width, int32_t* height);
            bool shouldClose();
            void swapBuffers();
            void destroy();
    };
}