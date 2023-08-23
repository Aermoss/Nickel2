#pragma once

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "transform.hpp"
#include "window.hpp"
#include "shader.hpp"

namespace nickel2 {
    class Transform;
    class Window;
    class Shader;
    
    class Camera {
        private:
            Window* window;

        public:
            float pitch, yaw, roll, fov, near, far;
            Transform* transform;
            glm::vec3 front, up;
            bool overrideMatrix;

            Camera(Window* window, float fov = 90.0f, float near = 0.01f, float far = 1000.0f, bool overrideMatrix = true);
            ~Camera();

            void updateMatrices(Shader* shader);
            Window* getWindow();
            void destroy();
    };

    class FirstPersonCamera : public Camera {
        private:
            Window* window;

        public:
            float sensitivity, normalSpeed, sprintSpeed;

            FirstPersonCamera(Window* window, float fov = 90.0f, float sensitivity = 100.0f, float near = 0.01f, float far = 1000.0f);
            ~FirstPersonCamera();

            void processInputs();
    };
}