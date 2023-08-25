#pragma once

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "transform.hpp"
#include "window.hpp"
#include "shader.hpp"

#undef near
#undef far

#define NICKEL2_EULER_OVERRIDE 0b0001
#define NICKEL2_INVERSE_MATRIX 0b0010
#define NICKEL2_CUSTOM_PROJECTION 0b0100
#define NICKEL2_HMD_POSITION 0b1000

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
            glm::mat4 proj;
            uint32_t flags;

            Camera(Window* window, float fov = 90.0f, float near = 0.01f, float far = 1000.0f, uint32_t flags = NICKEL2_EULER_OVERRIDE);
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

    class VirutalRealityCamera : public Camera {
        private:
            Window* window;

        public:
            VirutalRealityCamera(Window* window, float near = 0.01f, float far = 1000.0f);
            ~VirutalRealityCamera();

            void prepareForRendering(rvr::RVREye eye);
    };
}