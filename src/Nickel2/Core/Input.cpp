#include <Nickel2/nkpch.hpp>
#include <Nickel2/Core/Input.hpp>
#include <Nickel2/Core/Window.hpp>
#include <Nickel2/Core/Application.hpp>
#include <Nickel2/Event/MouseEvent.hpp>

#include <GLFW/glfw3.h>
#include <imgui/imgui_internal.h>

namespace Nickel2 {
    void Input::Initialize(GLFWwindow* windowHandle) {
        NK_CORE_ASSERT(windowHandle, "Window handle is null.");
        Input::windowHandle = windowHandle;
        Input::lastMousePosition = GetMousePosition();
    }

    void Input::Terminate() {
        Input::windowHandle = nullptr;
    }

    void Input::HandleEvent(Event& event) {
        switch (event.GetEventType()) {
            case EventType::MouseMove: {
                mousePosition = static_cast<MouseMoveEvent&>(event).GetPosition();
                deltaMousePosition = mousePosition - lastMousePosition, lastMousePosition = mousePosition;
                break;
            } case EventType::MouseScroll: {
                deltaMouseScroll = static_cast<MouseScrollEvent&>(event).GetOffset();
                mouseScroll += deltaMouseScroll;
                break;
            } default: {
                break;
            }
        }
    }

    KeyState Input::GetKeyState(KeyCode keyCode) {
        return static_cast<KeyState>(glfwGetKey(windowHandle, static_cast<int32_t>(keyCode)));
    }

    bool Input::IsKeyPressed(KeyCode keyCode) {
        return glfwGetKey(windowHandle, static_cast<int32_t>(keyCode)) == GLFW_PRESS;
    }

    bool Input::IsKeyHeld(KeyCode keyCode) {
        return glfwGetKey(windowHandle, static_cast<int32_t>(keyCode)) >= GLFW_PRESS;
    }

    bool Input::IsKeyReleased(KeyCode keyCode) {
        return glfwGetKey(windowHandle, static_cast<int32_t>(keyCode)) == GLFW_RELEASE;
    }

    KeyState Input::GetMouseButtonState(MouseButton button) {
        return static_cast<ButtonState>(glfwGetMouseButton(windowHandle, static_cast<int32_t>(button)));
    }

    bool Input::IsMouseButtonPressed(MouseButton button) {
        return glfwGetMouseButton(windowHandle, static_cast<int32_t>(button)) == GLFW_PRESS;
    }

    bool Input::IsMouseButtonHeld(MouseButton button) {
        return glfwGetMouseButton(windowHandle, static_cast<int32_t>(button)) >= GLFW_PRESS;
    }

    bool Input::IsMouseButtonReleased(MouseButton button) {
        return glfwGetMouseButton(windowHandle, static_cast<int32_t>(button)) == GLFW_RELEASE;
    }

    glm::vec2 Input::GetMouseScrollDelta() {
        return deltaMouseScroll;
    }

    glm::vec2 Input::GetMouseScroll() {
        return mouseScroll;
    }

    void Input::SetMouseScroll(const glm::vec2& scroll) {
        mouseScroll = scroll;
    }

    glm::vec2 Input::GetMousePositionDelta() {
        return deltaMousePosition;
    }

    glm::vec2 Input::GetMousePosition() {
        double x, y;
        glfwGetCursorPos(windowHandle, &x, &y);
        return { (float) x, (float) y };
    }

    void Input::SetMousePosition(const glm::vec2& position) {
        glfwSetCursorPos(windowHandle, position.x, position.y);
    }

    void Input::SetCursorMode(CursorMode mode) {
        glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL + (int32_t) mode);
    }

    CursorMode Input::GetCursorMode() {
        return static_cast<CursorMode>(glfwGetInputMode(windowHandle, GLFW_CURSOR) - GLFW_CURSOR_NORMAL);
    }
}