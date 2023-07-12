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

            FPSCamera(Window* window, glm::vec3 position, float fov, float sensitivity, float near, float far);
            ~FPSCamera();

            void processInputs();
            void updateMatrices(Shader* shader);
            void destroy();
    };
}