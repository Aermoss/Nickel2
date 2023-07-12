#include <iostream>
#include <vector>

#include "nickel2.hpp"

int main() {
    glfwInit();
    nickel2::Window window(1200, 600, "Nickel 2", true);

    nickel2::Shader shader(
        nickel2::readFile("shaders/default.vert"),
        nickel2::readFile("shaders/default.frag")
    );

    nickel2::FPSCamera camera(&window, glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, 100.0f, 0.1f, 100.0f);
    nickel2::Model model("res/deagle.obj");

    glClearColor(0.0f, 0.5f, 0.5f, 1.0f);

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
        shader.setUniform1i("enableIBL", 0);
        shader.setUniform1i("enableShadows", 0);
        shader.setUniform1i("useAlbedoMap", 1);
        shader.setUniform1i("useMetallicMap", 0);
        shader.setUniform1i("useRoughnessMap", 0);
        shader.setUniform1i("useNormalMap", 0);
        shader.setUniform1i("useAmbientMap", 0);
        shader.setUniform1i("useSpecularMap", 0);
        shader.setUniform3fv("albedoDefault", (float*) glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        shader.setUniform1f("metallicDefault", 0.5f);
        shader.setUniform1f("roughnessDefault", 0.5f);
        shader.setUniform1f("ambientDefault", 0.5f);

        shader.setUniform1i("lightCount", 1);
        shader.setUniform1f("lightBrightnesses[0]", 100.0f);
        shader.setUniform3fv("lightPositions[0]", (float*) glm::value_ptr(glm::vec3(5.0f, 5.0f, 5.0f)));
        shader.setUniform3fv("lightColors[0]", (float*) glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        shader.setUniformMatrix3fv("modelMatrix", (float*) glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(modelMatrix)))));
        shader.setUniformMatrix4fv("model", glm::value_ptr(modelMatrix));
        shader.unuse();

        camera.updateMatrices(&shader);
        model.render(&shader);
        window.swapBuffers();
    }

    glfwTerminate();
    return 0;
}