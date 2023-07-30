#include <iostream>
#include <vector>

#include "nickel2.hpp"

int main() {
    glfwInit();
    nickel2::Window window(1200, 600, "Nickel2", true);

    nickel2::Renderer renderer(&window, "res/hdr_textures/train_station.hdr");

    renderer.lights = {
        nickel2::Light(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 3.0f)
    };

    nickel2::FPSCamera camera(&window, glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, 100.0f, 0.01f, 100.0f);
    nickel2::Model model("res/deagle.obj");
    model.transform->rotate(glm::vec3(-15.0f, 45.0f, 45.0f));
    nickel2::Model ground("res/cube.obj");
    ground.transform->scale(glm::vec3(10.0f, 1.0f, 10.0f));
    ground.transform->translate(glm::vec3(0.0f, -2.0f, 0.0f));

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    while (!window.shouldClose()) {
        glfwPollEvents();
        window.update();
        window.clear();
        camera.processInputs();

        if (window.input->getKey(NICKEL2_KEY_Q)) {
            renderer.removeHDRTexture();
        }
        
        if (window.input->getKey(NICKEL2_KEY_E)) {
            renderer.loadHDRTexture("res/hdr_textures/square_night.hdr");
        }

        if (window.input->getKey(NICKEL2_KEY_R)) {
            renderer.reloadShaders();
            std::cout << "shaders reloaded" << std::endl;
        }

        // model.transform->rotate(glm::vec3(0.1f, 1.0f, 0.0f));

        renderer.submit(&model);
        renderer.submit(&ground);
        renderer.render(&camera);
        window.swapBuffers();
    }

    model.destroy();
    ground.destroy();
    camera.destroy();
    renderer.destroy();
    window.destroy();
    glfwTerminate();
    return 0;
}