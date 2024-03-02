#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/Camera.hpp>

namespace Nickel2 {
    Camera::Camera(Window* window, Transform* transform, float fov, float near, float far, uint32_t flags)
        : window(window), transform(transform), fov(fov), near(near), far(far), flags(flags), primary(true) {
        pitch = 0.0f, yaw = 0.0f, roll = 0.0f;
        front = glm::vec3(0.0f, 0.0f, 0.0f);
        up = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    Camera::~Camera() {

    }

    void Camera::updateMatrices(Shader* shader) {
        if ((flags & NICKEL2_CUSTOM_PROJECTION) == 0) {
            int32_t width, height;
            window->getSize(&width, &height);
            proj = glm::perspective(glm::radians(fov), (float) width / (float) height, near, far);
        }

        glm::vec3 position = transform->getTranslation();

        if ((flags & NICKEL2_EULER_OVERRIDE) != 0) {
            front = glm::normalize(glm::vec3(
                cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                sin(glm::radians(pitch)),
                sin(glm::radians(yaw)) * cos(glm::radians(pitch))
            ));

            transform->overrideMatrix(glm::lookAt(position, position + front, up));
        }

        transform->updateWorldMatrix();
        view = transform->getWorldMatrix();

        if ((flags & NICKEL2_INVERSE_MATRIX) != 0)
            view = glm::inverse(view);

        shader->use();
        shader->setUniform3fv("cameraPosition", glm::value_ptr(position));
        shader->setUniformMatrix4fv("proj", glm::value_ptr(proj));
        shader->setUniformMatrix4fv("view", glm::value_ptr(view));
        shader->unuse();
    }

    Window* Camera::getWindow() {
        return window;
    }

    glm::mat4 Camera::getProjection() {
        return proj;
    }

    glm::mat4 Camera::getView() {
        return view;
    }

    void Camera::setPrimary(bool primary) {
        this->primary = primary;
    }

    bool Camera::getPrimary() {
        return primary;
    }

    void Camera::destroy() {

    }
}