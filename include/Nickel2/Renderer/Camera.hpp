#pragma once

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Core/Window.hpp"
#include "Shader.hpp"

#define NK_LOOK_AT 0b0001
#define NK_EULER_OVERRIDE 0b0010
#define NK_INVERSE_MATRIX 0b0100
#define NK_CUSTOM_PROJECTION 0b1000

namespace Nickel2 {
    class Transform;
    class Entity;
    class Window;
    class Shader;

    class Camera {
        private:
            Entity* entity;
            Window* window;

        public:
            double pitch, yaw, roll;
            float fov, nearPlane, farPlane;
            glm::vec3 front, up, target;
            glm::mat4 proj, view;
            uint32_t flags;
            bool primary;

            Camera(Entity* entity, Window* window, float fov = 90.0f, float nearPlane = 0.01f, float farPlane = 1000.0f, uint32_t flags = NK_EULER_OVERRIDE);
            ~Camera();

            void UpdateMatrices(std::shared_ptr<Shader> shader);
            Window* GetWindow();
            glm::mat4 GetProjection();
            glm::mat4 GetView();
            glm::vec3 GetFront();
            glm::vec3 GetRight();
            glm::vec3 GetUp();
            void SetPrimary(bool primary);
            bool GetPrimary();
            void Destroy();
    };
}