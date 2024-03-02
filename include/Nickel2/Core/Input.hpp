#pragma once

#include <glfw/glfw3.h>

#include "Constants.hpp"

namespace Nickel2 {
    namespace input {
        extern double scrollX, scrollY;
        void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    }

    class InputHandler {
        private:
            GLFWwindow* window;
            double &scrollX, &scrollY;

        public:
            InputHandler(GLFWwindow* window);
            ~InputHandler();

            bool getKey(int32_t key);
            bool getMouseButton(int32_t button);
            void getCursorPosition(double* x, double* y);
            void setCursorPosition(double x, double y);
            void setCursorVisible(bool visible);
            void destroy();
    };
}