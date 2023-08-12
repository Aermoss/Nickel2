#include <iostream>
#include <vector>

#include <nickel2/nickel2.hpp>

int main() {
    nickel2::Context context;
    nickel2::Window window(1200, 600, "Nickel2", nickel2::Color(0, 0, 0), true);

    nickel2::Renderer renderer(&window, "res/hdr_textures/train_station.hdr");
    nickel2::FirstPersonCamera camera(&window, 90.0f, 100.0f, 0.01f, 1000.0f);

    nickel2::Scene scene;

    scene.lights = {
        nickel2::Light(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(1.0f, 1.0f, 1.0f), 3.0f)
    };

    nickel2::Model model("res/deagle.obj");
    model.transform->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    model.transform->setRotation(glm::vec3(0.0f, 180.0f, 0.0f));
    model.transform->setScale(glm::vec3(1.0f));
    scene.submit(&model);

    nickel2::Model ground("res/cube.obj");
    ground.transform->setPosition(glm::vec3(0.0f, -2.0f, 0.0f));
    ground.transform->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    ground.transform->setScale(glm::vec3(10.0f, 1.0f, 10.0f));
    scene.submit(&ground);

    nickel2::Scene scene2;

    scene2.lights = {
        nickel2::Light(glm::vec3(5.0f, 10.0f, 5.0f), glm::vec3(0.0f, 1.0f, 1.0f), 3.0f),
        nickel2::Light(glm::vec3(-5.0f, 2.0f, -5.0f), glm::vec3(1.0f, 0.0f, 1.0f), 3.0f)
    };

    nickel2::Model model2("res/ak-12.obj");
    model2.transform->setPosition(glm::vec3(0.0f, 0.5f, 0.0f));
    model2.transform->setScale(glm::vec3(3.0f));
    scene2.submit(&model2);

    nickel2::Model ground2("res/cube.obj");
    ground2.transform->setPosition(glm::vec3(0.0f, -2.0f, 0.0f));
    ground2.transform->setScale(glm::vec3(10.0f, 1.0f, 10.0f));
    scene2.submit(&ground2);

    float fov = 45.0f;
    float lastScrollY = 0.0;
    float interpolatedPitch = 0.0f, interpolatedYaw = 0.0f;
    float realPitch = 0.0f, realYaw = 0.0f;

    nickel2::Scene* currentScene = &scene;

    while (!window.shouldClose()) {
        context.pollEvents();
        window.update();
        window.clear();

        camera.pitch = realPitch;
        camera.yaw = realYaw;
        camera.processInputs();
        realPitch = camera.pitch;
        realYaw = camera.yaw;

        interpolatedPitch = glm::lerp(interpolatedPitch, realPitch, 0.2f);
        interpolatedYaw = glm::lerp(interpolatedYaw, realYaw, 0.2f);
        camera.pitch = interpolatedPitch;
        camera.yaw = interpolatedYaw;

        double change = lastScrollY - nickel2::input::scrollY;
        lastScrollY = nickel2::input::scrollY;

        fov += (float) change * 3.0f;
        if (fov > 150.0f) fov = 150.0f;
        if (fov < 5.0f) fov = 5.0f;

        camera.fov = glm::lerp(camera.fov, fov, 0.2f);

        if (window.input->getKey(NICKEL2_KEY_X)) {
            currentScene = &scene;
        }
        
        if (window.input->getKey(NICKEL2_KEY_C)) {
            currentScene = &scene2;
        }

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

        renderer.render(&camera, currentScene);
        window.swapBuffers();
    }

    scene.destroy();
    scene2.destroy();
    camera.destroy();
    renderer.destroy();
    window.destroy();
    context.destroy();
    return 0;
}