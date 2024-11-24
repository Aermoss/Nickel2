#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/Camera.hpp>
#include <Nickel2/Core/Application.hpp>
#include <Nickel2/Scene/Entity.hpp>

namespace Nickel2 {
    Camera::Camera(Entity* entity, float fov, float nearClip, float farClip, uint32_t flags)
        : entity(entity), flags(flags), fov(fov), nearClip(nearClip), farClip(farClip) {}

    Camera::~Camera() {}

    void Camera::UpdateMatrices(std::shared_ptr<Shader> shader) {
        glm::vec3 translation = entity->GetComponent<TransformComponent>().GetTranslation();

        if ((flags & NK_CUSTOM_PROJECTION) == 0) {
            glm::vec2 size = Application::Get().GetWindow().GetSize();
            proj = glm::perspective(glm::radians(fov), static_cast<float>(size.x) / static_cast<float>(size.y), nearClip, farClip);
        }

        if ((flags & NK_EULER_OVERRIDE) != 0) {
            front = glm::normalize(this->GetFront());
            view = glm::lookAt(translation, translation + front, glm::vec3(0.0f, 1.0f, 0.0f));
        } else if ((flags & NK_LOOK_AT) != 0) {
            view = glm::lookAt(translation, target, up);
        } else {
            view = entity->GetComponent<TransformComponent>().GetWorldTransform();
        }

        if ((flags & NK_INVERSE_MATRIX) != 0)
            view = glm::inverse(view);

        shader->Bind();
        shader->SetFloat3("cameraPosition", translation);
        shader->SetMat4("proj", proj);
        shader->SetMat4("view", view);
        shader->Unbind();
    }

    glm::vec3 Camera::GetFront() const {
        return glm::vec3(
            glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
            glm::sin(glm::radians(pitch)),
            glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch))
        );
    }

    glm::vec3 Camera::GetRight() const {
        return glm::vec3(
            glm::cos(glm::radians(yaw)), 0.0f, -glm::sin(glm::radians(yaw))
        );
    }

    glm::vec3 Camera::GetUp() const {
        return glm::vec3(
            glm::sin(glm::radians(pitch)) * glm::sin(glm::radians(yaw)),
            glm::cos(glm::radians(pitch)),
            glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw))
        );
    }
}