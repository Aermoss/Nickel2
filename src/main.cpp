#include <iostream>
#include <vector>

#include "nickel2.hpp"

int main() {
    glfwInit();
    nickel2::Window window(1200, 600, "Nickel2", true);

    nickel2::Shader shader(
        nickel2::readFile("shaders/default.vert"),
        nickel2::readFile("shaders/default.frag")
    );

    nickel2::FPSCamera camera(&window, glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, 100.0f, 0.1f, 100.0f);
    camera.sprintSpeed = 10.0f;
    nickel2::Model model("res/deagle.obj");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    while (!window.shouldClose()) {
        glfwPollEvents();
        window.update();
        window.clear();
        camera.processInputs();

        if (window.input->getKey(NICKEL2_KEY_R)) {
            shader.reload(
                nickel2::readFile("shaders/default.vert"),
                nickel2::readFile("shaders/default.frag")
            );

            std::cout << "shaders reloaded" << std::endl;
        }

        shader.use();
        shader.setUniform1i("lightCount", 1);
        shader.setUniform1f("lightBrightnesses[0]", 1.0f);
        shader.setUniform3fv("lightPositions[0]", (float*) glm::value_ptr(glm::vec3(3.0f, 3.0f, 3.0f)));
        shader.setUniform3fv("lightColors[0]", (float*) glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        shader.unuse();

        model.transform->rotate(glm::vec3(0.1f, 1.0f, 0.0f));

        camera.updateMatrices(&shader);
        model.render(&shader);
        window.swapBuffers();
    }

    shader.destroy();
    model.destroy();
    camera.destroy();
    window.destroy();
    glfwTerminate();
    return 0;
}