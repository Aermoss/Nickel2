#include <nickel2/camera.hpp>

namespace nickel2 {
    FPSCamera::FPSCamera(Window* window, glm::vec3 position, float fov, float sensitivity, float near, float far)
        : window(window), position(position), fov(fov), sensitivity(sensitivity), near(near), far(far) {
        normalSpeed = 0.1f, sprintSpeed = 0.2f;
        pitch = 0.1f, yaw = 0.1f, roll = 0.1f;
        up = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    FPSCamera::~FPSCamera() {

    }

    void FPSCamera::processInputs() {
        float speed = window->input->getKey(NICKEL2_KEY_LEFT_CONTROL) ? sprintSpeed : normalSpeed;

        if (window->input->getKey(NICKEL2_KEY_W)) {
            position += speed * glm::vec3(glm::cos(glm::radians(yaw)), 0.0f, glm::sin(glm::radians(yaw))) * window->deltaTime * 10.0f;
        }
        
        if (window->input->getKey(NICKEL2_KEY_S)) {
            position += speed * -glm::vec3(glm::cos(glm::radians(yaw)), 0.0f, glm::sin(glm::radians(yaw))) * window->deltaTime * 10.0f;
        }

        if (window->input->getKey(NICKEL2_KEY_A)) {
            position += speed * -glm::normalize(glm::cross(front, up)) * window->deltaTime * 10.0f;
        }

        if (window->input->getKey(NICKEL2_KEY_D)) {
            position += speed * glm::normalize(glm::cross(front, up)) * window->deltaTime * 10.0f;
        }

        if (window->input->getKey(NICKEL2_KEY_SPACE)) {
            position += speed * up * window->deltaTime * 10.0f;
        }

        if (window->input->getKey(NICKEL2_KEY_LEFT_SHIFT)) {
            position += speed * -up * window->deltaTime * 10.0f;
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

    void FPSCamera::updateMatrices(Shader* shader) {
        int32_t width, height;
        window->getSize(&width, &height);

        front = glm::normalize(glm::vec3(
            cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
            sin(glm::radians(pitch)),
            sin(glm::radians(yaw)) * cos(glm::radians(pitch))
        ));

        glm::mat4 proj = glm::perspective(glm::radians(fov), (float) width / (float) height, near, far);
        glm::mat4 view = glm::lookAt(position, position + front, up);

        shader->use();
        shader->setUniform3fv("cameraPosition", glm::value_ptr(position));
        shader->setUniformMatrix4fv("proj", glm::value_ptr(proj));
        shader->setUniformMatrix4fv("view", glm::value_ptr(view));
        shader->unuse();
    }

    void FPSCamera::destroy() {

    }
}
