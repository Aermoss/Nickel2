#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/Camera.hpp>
#include <Nickel2/Scene/Entity.hpp>

namespace Nickel2 {
    Camera::Camera(Entity* entity, Window* window, float fov, float nearPlane, float farPlane, uint32_t flags)
        : entity(entity), window(window), fov(fov), nearPlane(nearPlane), farPlane(farPlane), flags(flags), primary(true) {
        pitch = 0.0, yaw = 0.0, roll = 0.0;
        front = glm::vec3(0.0f, 0.0f, 0.0f);
        up = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    Camera::~Camera() {

    }

    void Camera::UpdateMatrices(std::shared_ptr<Shader> shader) {
        glm::vec3 translation = entity->GetComponent<TransformComponent>().GetTranslation();

        if ((flags & NK_CUSTOM_PROJECTION) == 0) {
            glm::vec2 size = window->GetSize();
            proj = glm::perspective(glm::radians(fov), (float) size.x / (float) size.y, nearPlane, farPlane);
        }

        if ((flags & NK_EULER_OVERRIDE) != 0) {
            front = glm::normalize(GetFront());
            view = glm::lookAt(translation, translation + front, up);
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

    Window* Camera::GetWindow() {
        return window;
    }

    glm::mat4 Camera::GetProjection() {
        return proj;
    }

    glm::mat4 Camera::GetView() {
        return view;
    }

    glm::vec3 Camera::GetFront() {
        return glm::vec3(
            glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
            glm::sin(glm::radians(pitch)),
            glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch))
        );
    }

    glm::vec3 Camera::GetRight() {
        return glm::vec3(
            glm::cos(glm::radians(yaw)), 0.0f, -glm::sin(glm::radians(yaw))
        );
    }

    glm::vec3 Camera::GetUp() {
        return glm::vec3(
            glm::sin(glm::radians(pitch)) * glm::sin(glm::radians(yaw)),
            glm::cos(glm::radians(pitch)),
            glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw))
        );
    }

    void Camera::SetPrimary(bool primary) {
        this->primary = primary;
    }

    bool Camera::GetPrimary() {
        return primary;
    }

    void Camera::Destroy() {

    }
}