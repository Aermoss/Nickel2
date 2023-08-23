#include <nickel2/input.hpp>

namespace nickel2 {
    namespace input {
        double scrollX = 0.0, scrollY = 0.0;
        
        void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
            scrollX += xoffset;
            scrollY += yoffset;
        }
    }

    InputHandler::InputHandler(GLFWwindow* window) : window(window), scrollX(input::scrollX), scrollY(input::scrollY) {
        glfwSetScrollCallback(window, input::scrollCallback);
    }

    InputHandler::~InputHandler() {

    }

    bool InputHandler::getKey(int32_t key) {
        return glfwGetKey(window, key);
    }

    bool InputHandler::getMouseButton(int32_t button) {
        return glfwGetMouseButton(window, button);
    }

    void InputHandler::getCursorPosition(double* x, double* y) {
        glfwGetCursorPos(window, x, y);
    }

    void InputHandler::setCursorPosition(double x, double y) {
        glfwSetCursorPos(window, x, y);
    }

    void InputHandler::setCursorVisible(bool visible) {
        glfwSetInputMode(window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
    }

    void InputHandler::destroy() {

    }
}