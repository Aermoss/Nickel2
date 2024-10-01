#pragma once

#include "KeyCodes.hpp"
#include "../Event/Event.hpp"

#include <glfw/glfw3.h>

#include <map>
#include <string_view>
#include <vector>

namespace Nickel2 {
    class Input {
        private:
            inline static GLFWwindow* windowHandle = nullptr;
            inline static glm::vec2 mousePosition, lastMousePosition, deltaMousePosition;
            inline static glm::vec2 mouseScroll, deltaMouseScroll;

        public:
            static void Initialize(GLFWwindow* windowHandle);
            static void Terminate();

            static void HandleEvent(Event& event);

            static KeyState GetKeyState(KeyCode keyCode);
            static bool IsKeyPressed(KeyCode keyCode);
            static bool IsKeyHeld(KeyCode keyCode);
            static bool IsKeyReleased(KeyCode keyCode);

            static ButtonState GetMouseButtonState(MouseButton button);
            static bool IsMouseButtonPressed(MouseButton button);
            static bool IsMouseButtonHeld(MouseButton button);
            static bool IsMouseButtonReleased(MouseButton button);

            static glm::vec2 GetMouseScrollDelta();
            static glm::vec2 GetMouseScroll();
            static void SetMouseScroll(const glm::vec2& scroll);

            static glm::vec2 GetMousePositionDelta();
            static glm::vec2 GetMousePosition();
            static void SetMousePosition(const glm::vec2& position);

            static void SetCursorMode(CursorMode mode);
            static CursorMode GetCursorMode();
    };
}