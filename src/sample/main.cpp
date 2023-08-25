#include <iostream>
#include <vector>

#include <nickel2/nickel2.hpp>

int main() {
    nickel2::Context context;
    nickel2::Window window(1200, 600, "Nickel2", nickel2::Color(0, 0, 0), true);
    nickel2::Renderer renderer(&window, "res/hdr_textures/syferfontein_18d_clear_puresky_4k.hdr");
    nickel2::FirstPersonCamera camera(&window, 90.0f, 100.0f, 0.1f, 100.0f);
    nickel2::Scene scene;

    scene.lights = {
        nickel2::Light(glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(1.0f, 1.0f, 1.0f), 100.0f)
    };

    nickel2::Model model("C:\\Users\\yusuf\\Downloads\\teknofest\\Radium\\res\\goynuk\\goynuk.obj");
    model.transform->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    model.transform->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    model.transform->setScale(glm::vec3(1.0f));
    scene.submit(&model);

    for (auto& [key, value] : model.getMeshes()) {
        std::cout << key << " " << value->material.name << std::endl;

        if (std::string(key).find("Wood_Cherry_Original", 2) != std::string::npos) {
            value->material.rotateTexture = true;
            value->material.textureScale = 20;
        } else if (std::string(key).find("Leaf_autumn", 2) != std::string::npos) {
            value->material.textureScale = 20;
        } else if (std::string(key).find("mia_material_x5SG", 2) != std::string::npos) {
            value->material.textureScale = 2;
        } else if (std::string(key).find("Material.015", 2) != std::string::npos) {
            value->material.textureScale = 20;
            value->material.metallic = 0.0f;
            value->material.roughness = 0.75f;
        } else if (std::string(key).find("Plane.008", 2) != std::string::npos) {
            value->material.textureScale = 20;
            value->material.metallic = 0.0f;
            value->material.roughness = 0.75f;
        } else if (std::string(key).find("Plane.009", 2) != std::string::npos) {
            value->material.textureScale = 50;
            value->material.metallic = 0.0f;
            value->material.roughness = 0.75f;
        } else if (std::string(key).find("Plane.001", 2) != std::string::npos) {
            value->material.textureScale = 50;
            value->material.metallic = 0.0f;
            value->material.roughness = 0.75f;
        } else if (std::string(key).find("Plane.003", 2) != std::string::npos) {
            value->material.rotateTexture = true;
            value->material.textureScale = 15;
            value->material.metallic = 0.0f;
            value->material.roughness = 0.75f;
        } else if (std::string(key).find("Plane.028", 2) != std::string::npos) {
            value->material.rotateTexture = true;
            value->material.textureScale = 20;
            value->material.metallic = 0.0f;
            value->material.roughness = 0.75f;
        } else if (std::string(key).find("Anit", 2) != std::string::npos) {
            value->material.rotateTexture = false;
            value->material.textureScale = 20;
            value->material.metallic = 0.0f;
            value->material.roughness = 0.75f;
        } else if (std::string(key).find("Saat_Kulesi", 2) != std::string::npos) {
            value->material.rotateTexture = true;
            value->material.textureScale = 20;
            value->material.metallic = 0.0f;
            value->material.roughness = 0.75f;
        } else if (std::string(key).find("Saat", 2) != std::string::npos) {
            value->material.rotateTexture = true;
            value->material.textureScale = 20;
            value->material.metallic = 0.0f;
            value->material.roughness = 0.75f;
        } else if (std::string(key).find("Material.007", 2) != std::string::npos) {
            value->material.rotateTexture = false;
            value->material.textureScale = 1;
            value->material.metallic = 0.0f;
            value->material.roughness = 0.75f;
        } else if (std::string(key).find("Plane.030", 2) != std::string::npos) {
            value->material.rotateTexture = false;
            value->material.textureScale = 1;
            value->material.metallic = 0.0f;
            value->material.roughness = 0.75f;
        } else if (std::string(key).find("Material.003", 2) != std::string::npos) {
            value->material.rotateTexture = false;
            value->material.textureScale = 20;
            value->material.metallic = 0.0f;
            value->material.roughness = 0.75f;
        }
    }

    nickel2::Scene* currentScene = &scene;
    nickel2::audio::Listener listener(&camera);
    nickel2::audio::Source source(&listener, "res/music.wav", true);
    source.play();

    float shadowPassTimer = 0.0f;

    while (!window.shouldClose()) {
        context.pollEvents();
        context.makeCurrent();
        listener.update();
        window.update();
        camera.processInputs();

        /* float speed = 0.2f;
        glm::vec3 d = rvr::RVRGetHmdDirection();
        float yaw = -std::atan2(d[0], d[2]) - glm::radians(90.0f);
        glm::vec3 front = glm::vec3(glm::cos(yaw), 0.0f, glm::sin(yaw));
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec2 joystickPosition = rvr::RVRGetControllerJoystickPosition(rvr::RVRControllerRight);

        if (joystickPosition.y != 0.0f) {
            camera.transform->translate(glm::vec3(front.x * speed * joystickPosition.y, 0.0f, front.z * speed * joystickPosition.y) * window.deltaTime * 10.0f);
        }

        if (joystickPosition.x != 0.0f) {
            glm::vec3 vector = normalize(cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
            camera.transform->translate(glm::vec3(vector.x * speed * joystickPosition.x, 0.0f, vector.z * speed * joystickPosition.x) * window.deltaTime * 10.0f);
        } */

        if ((shadowPassTimer += window.deltaTime) > 1.0f) {
            shadowPassTimer = 0.0f;
            renderer.updateShadowMap(&scene);
        }

        if (window.input->getKey(NICKEL2_KEY_Q)) {
            renderer.removeHDRTexture();
        }
        
        if (window.input->getKey(NICKEL2_KEY_1)) {
            renderer.loadHDRTexture("res/hdr_textures/evening_road_01_puresky_4k.hdr");
        }

        if (window.input->getKey(NICKEL2_KEY_2)) {
            renderer.loadHDRTexture("res/hdr_textures/kloofendal_43d_clear_puresky_4k.hdr");
        }

        if (window.input->getKey(NICKEL2_KEY_3)) {
            renderer.loadHDRTexture("res/hdr_textures/kloofendal_48d_partly_cloudy_puresky_4k.hdr");
        }
        
        if (window.input->getKey(NICKEL2_KEY_4)) {
            renderer.loadHDRTexture("res/hdr_textures/rustig_koppie_puresky_4k.hdr");
        }

        if (window.input->getKey(NICKEL2_KEY_5)) {
            renderer.loadHDRTexture("res/hdr_textures/syferfontein_18d_clear_puresky_4k.hdr");
        }

        if (window.input->getKey(NICKEL2_KEY_R)) {
            renderer.reloadShaders();
            std::cout << "shaders reloaded" << std::endl;
        }

        window.clear();
        renderer.render(&camera, currentScene, false);
        window.swapBuffers();
    }

    scene.destroy();
    camera.destroy();
    renderer.destroy();
    window.destroy();
    context.destroy();
    return 0;
}