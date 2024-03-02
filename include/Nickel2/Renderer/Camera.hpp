#pragma once

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Core/Constants.hpp"
#include "../Core/Window.hpp"
#include "../Scene/Transform.hpp"

#include "Shader.hpp"

#if defined(near) || defined(far)
#undef near
#undef far
#endif

namespace Nickel2 {
    class Transform;
    class Window;
    class Shader;
    
    class Camera {
        private:
            Window* window;

        public:
            Transform* transform;
            float pitch, yaw, roll, fov, near, far;
            glm::vec3 front, up;
            glm::mat4 proj, view;
            uint32_t flags;
            bool primary;

            Camera(Window* window, Transform* transform, float fov = 90.0f, float near = 0.01f, float far = 1000.0f, uint32_t flags = NICKEL2_EULER_OVERRIDE);
            ~Camera();

            void updateMatrices(Shader* shader);
            Window* getWindow();
            glm::mat4 getProjection();
            glm::mat4 getView();
            void setPrimary(bool primary);
            bool getPrimary();
            void destroy();
    };
}