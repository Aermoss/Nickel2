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
    class Entity;
    class Shader;

    class Camera {
        private:
            Entity* entity;
            bool primary = true;
            glm::mat4 proj, view;
            uint32_t flags = 0;

        public:
            float pitch, yaw, roll, fov, nearClip, farClip;
            glm::vec3 target, front = { 0.0f, 0.0f, 0.0f }, up = { 0.0f, 1.0f, 0.0f };

            Camera(Entity* entity, float fov = 90.0f, float nearPlane = 0.1f, float farPlane = 100.0f, uint32_t flags = NK_EULER_OVERRIDE);
            ~Camera();

            void UpdateMatrices(std::shared_ptr<Shader> shader);

            glm::vec3 GetFront() const;
            glm::vec3 GetRight() const;
            glm::vec3 GetUp() const;

            void SetProjection(const glm::mat4& proj) { this->proj = proj; }
            glm::mat4 GetProjection() const { return proj; }

            void SetView(const glm::mat4& view) { this->view = view; }
            glm::mat4 GetView() const { return view; }

            void SetFlags(uint32_t flags) { this->flags = flags; }
            uint32_t GetFlags() const { return flags; }

            void SetPrimary(bool primary) { this->primary = primary; }
            bool GetPrimary() const { return primary; }

            friend class Scene;
    };
}