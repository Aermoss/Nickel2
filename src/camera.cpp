#include <nickel2/camera.hpp>

namespace nickel2 {
    Camera::Camera(Window* window, float fov, float near, float far, bool overrideMatrix)
        : window(window), fov(fov), near(near), far(far), overrideMatrix(overrideMatrix) {
        pitch = 0.0f, yaw = 0.0f, roll = 0.0f;
        transform = new Transform();
        front = glm::vec3(0.0f, 0.0f, 0.0f);
        up = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    Camera::~Camera() {

    }

    void Camera::updateMatrices(Shader* shader) {
        int32_t width, height;
        window->getSize(&width, &height);

        glm::mat4 proj = glm::perspective(glm::radians(fov), (float) width / (float) height, near, far);
        glm::vec3 position = transform->getPosition();

        if (overrideMatrix) {
            front = glm::normalize(glm::vec3(
                cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                sin(glm::radians(pitch)),
                sin(glm::radians(yaw)) * cos(glm::radians(pitch))
            ));

            transform->overrideMatrix(glm::lookAt(position, position + front, up));
        }

        transform->updateWorldMatrix();
        glm::mat4 view = transform->getWorldMatrix();
        if (!overrideMatrix) view = glm::inverse(view);

        shader->use();
        shader->setUniform3fv("cameraPosition", glm::value_ptr(position));
        shader->setUniformMatrix4fv("proj", glm::value_ptr(proj));
        shader->setUniformMatrix4fv("view", glm::value_ptr(view));
        shader->unuse();
    }

    Window* Camera::getWindow() {
        return window;
    }

    void Camera::destroy() {
        transform->destroy();
        delete transform;
    }

    FirstPersonCamera::FirstPersonCamera(Window* window, float fov, float sensitivity, float near, float far)
        : Camera(window, fov, near, far, true), window(window), sensitivity(sensitivity) {
        normalSpeed = 0.1f, sprintSpeed = 0.2f;
    }

    FirstPersonCamera::~FirstPersonCamera() {
        
    }

    void FirstPersonCamera::processInputs() {
        float speed = window->input->getKey(NICKEL2_KEY_LEFT_CONTROL) ? sprintSpeed : normalSpeed;

        if (window->input->getKey(NICKEL2_KEY_W)) {
            transform->translate(speed * glm::vec3(glm::cos(glm::radians(yaw)), 0.0f, glm::sin(glm::radians(yaw))) * window->deltaTime * 10.0f);
        }
        
        if (window->input->getKey(NICKEL2_KEY_S)) {
            transform->translate(speed * -glm::vec3(glm::cos(glm::radians(yaw)), 0.0f, glm::sin(glm::radians(yaw))) * window->deltaTime * 10.0f);
        }

        if (window->input->getKey(NICKEL2_KEY_A)) {
            transform->translate(speed * -glm::normalize(glm::cross(front, up)) * window->deltaTime * 10.0f);
        }

        if (window->input->getKey(NICKEL2_KEY_D)) {
            transform->translate(speed * glm::normalize(glm::cross(front, up)) * window->deltaTime * 10.0f);
        }

        if (window->input->getKey(NICKEL2_KEY_SPACE)) {
            transform->translate(speed * up * window->deltaTime * 10.0f);
        }

        if (window->input->getKey(NICKEL2_KEY_LEFT_SHIFT)) {
            transform->translate(speed * -up * window->deltaTime * 10.0f);
        }

        double x, y;
        window->input->setCursorVisible(false);
        window->input->getCursorPosition(&x, &y);

        int32_t width, height;
        window->getSize(&width, &height);

        float xOffset = sensitivity * (x - (int) (width / 2)) / width;
        float yOffset = sensitivity * (y - (int) (height / 2)) / height;

        yaw += xOffset;
        pitch -= yOffset;

        if (pitch >= 89.99f)
            pitch = 89.99f;

        if (pitch <= -89.99f)
            pitch = -89.99f;

        window->input->setCursorPosition(width / 2, height / 2);
    }
}