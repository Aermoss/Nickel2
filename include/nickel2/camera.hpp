#pragma once

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window.hpp"
#include "shader.hpp"

namespace nickel2 {
    class FPSCamera {
        private:
            Window* window;

        public:
            float sprintSpeed, normalSpeed;
            float pitch, yaw, roll, sensitivity, near, far, fov;
            glm::vec3 position, front, up;

            FPSCamera(Window* window, glm::vec3 position = {0.0f, 0.0f, 0.0f}, float fov = 90.0f, float sensitivity = 100.0f, float near = 0.01f, float far = 1000.0f);
            ~FPSCamera();

            void processInputs();
            void updateMatrices(Shader* shader);
            void destroy();
    };
}