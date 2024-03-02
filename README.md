# Nickel2
A game engine written in C++ using OpenGL.

# Examples
## Creating a window.
```c++
#include <Nickel2/nkpch.hpp>
#include <Nickel2/Nickel2.hpp>

int main(int argc, const char* argv[]) {
    Nickel2::Context context;
    Nickel2::Window window(1200, 600, "Nickel2", Nickel2::Color(0, 0, 0));

    while (!window.shouldClose()) {
        context.pollEvents();
        context.makeCurrent();
        window.update();
        window.clear();
        window.swapBuffers();
    }

    window.destroy();
    context.destroy();
    return 0;
}
```

## Creating an ImGui window.
```c++
#include <Nickel2/nkpch.hpp>
#include <Nickel2/Nickel2.hpp>

int main(int argc, const char* argv[]) {
    Nickel2::Context context;
    Nickel2::Window window(1200, 600, "Nickel2", Nickel2::Color(0, 0, 0));
    Nickel2::Renderer renderer(&window, "res/hdr_textures/kloppenheim_07_puresky_4k.hdr");
    Nickel2::Scene scene;

    Nickel2::Entity camera(&scene);
    camera.addComponent<Nickel2::CameraComponent>(&window, 90.0f, 1.0f, 1000.0f, NICKEL2_EULER_OVERRIDE);
    camera.addComponent<Nickel2::ListenerComponent>(camera.getComponent<Nickel2::CameraComponent>());
    scene.setPrimaryCamera(&camera);

    Nickel2::Scene* currentScene = &scene;

    while (!window.shouldClose()) {
        context.pollEvents();
        context.makeCurrent();
        window.update();

        camera.getComponent<Nickel2::CameraComponent>()->yaw += 2.0f * window.deltaTime;
        currentScene->update(window.deltaTime);

        if (window.input->getKey(NICKEL2_KEY_R))
            renderer.reloadShaders();

        renderer.imGuiNewFrame();

        ImGui::Begin("Nickel2");
        ImGui::Text("Hello, world!");
        ImGui::End();

        window.clear();
        renderer.render(currentScene, true, true, true);
        window.swapBuffers();
    }

    scene.destroy();
    renderer.destroy();
    window.destroy();
    context.destroy();
    return 0;
}
```

## Rendering a model.
```c++
#include <Nickel2/nkpch.hpp>
#include <Nickel2/Nickel2.hpp>

class FirstPersonCamera : public Nickel2::Entity {
    private:
        Nickel2::Window* window;

    public:
        float sensitivity, normalSpeed, sprintSpeed;
        float targetFov, lastScrollY, realYaw, realPitch, interpolatedYaw, interpolatedPitch;

        FirstPersonCamera(Nickel2::Scene* scene, Nickel2::Window* window, float fov, float sensitivity, float near, float far)
            : Entity(scene), window(window), sensitivity(sensitivity), normalSpeed(0.1f), sprintSpeed(0.2f), targetFov(0.0f),
            lastScrollY(0.0f), realYaw(0.0f), realPitch(0.0f), interpolatedYaw(0.0f), interpolatedPitch(0.0f) {
            addComponent<Nickel2::CameraComponent>(window, fov, near, far, NICKEL2_EULER_OVERRIDE);
        }

        ~FirstPersonCamera() {}

        void onCreate() override {}
        void onDestroy() override {}

        void onUpdate(float deltaTime) override {
            if (window->input->getKey(NICKEL2_KEY_LEFT_ALT)) {
                window->input->setCursorVisible(true);
                return;
            }

            if (!glfwGetWindowAttrib(window->getGLFWWindow(), GLFW_FOCUSED)) return;
            float speed = window->input->getKey(NICKEL2_KEY_LEFT_CONTROL) ? sprintSpeed : normalSpeed;
            Nickel2::Transform* transform = getComponent<Nickel2::TransformComponent>().transform;
            Nickel2::Camera* camera = getComponent<Nickel2::CameraComponent>().camera;

            if (window->input->getKey(NICKEL2_KEY_W)) {
                transform->translate(speed * glm::vec3(glm::cos(glm::radians(camera->yaw)), 0.0f, glm::sin(glm::radians(camera->yaw))) * window->deltaTime * 10.0f);
            }
            
            if (window->input->getKey(NICKEL2_KEY_S)) {
                transform->translate(speed * -glm::vec3(glm::cos(glm::radians(camera->yaw)), 0.0f, glm::sin(glm::radians(camera->yaw))) * window->deltaTime * 10.0f);
            }

            if (window->input->getKey(NICKEL2_KEY_A)) {
                transform->translate(speed * -glm::normalize(glm::cross(camera->front, camera->up)) * window->deltaTime * 10.0f);
            }

            if (window->input->getKey(NICKEL2_KEY_D)) {
                transform->translate(speed * glm::normalize(glm::cross(camera->front, camera->up)) * window->deltaTime * 10.0f);
            }

            if (window->input->getKey(NICKEL2_KEY_SPACE)) {
                transform->translate(speed * camera->up * window->deltaTime * 10.0f);
            }

            if (window->input->getKey(NICKEL2_KEY_LEFT_SHIFT)) {
                transform->translate(speed * -camera->up * window->deltaTime * 10.0f);
            }

            double x, y;
            window->input->setCursorVisible(false);
            window->input->getCursorPosition(&x, &y);

            int32_t width, height;
            window->getSize(&width, &height);

            float xOffset = sensitivity * (x - (int32_t) (width / 2)) / width;
            float yOffset = sensitivity * (y - (int32_t) (height / 2)) / height;

            camera->yaw = realYaw;
            camera->pitch = realPitch;

            camera->yaw += xOffset;
            camera->pitch -= yOffset;

            camera->pitch = glm::clamp(camera->pitch, -89.99f, 89.99f);

            realYaw = camera->yaw;
            realPitch = camera->pitch;

            interpolatedPitch = glm::lerp(interpolatedPitch, realPitch, 0.2f * 50.0f * window->deltaTime);
            interpolatedYaw = glm::lerp(interpolatedYaw, realYaw, 0.2f * 50.0f * window->deltaTime);
            camera->pitch = interpolatedPitch;
            camera->yaw = interpolatedYaw;

            float change = lastScrollY - Nickel2::input::scrollY;
            lastScrollY = Nickel2::input::scrollY;

            targetFov = glm::clamp((targetFov += change * 5.0f), 10.0f, 130.0f);
            camera->fov = glm::lerp(camera->fov, targetFov, 0.15f * 50.0f * window->deltaTime);
            sensitivity = camera->fov * 1.5f + 10.0f;

            window->input->setCursorPosition(width / 2, height / 2);
        }
};

int main(int argc, const char* argv[]) {
    Nickel2::Context context;
    Nickel2::Window window(1200, 600, "Nickel2", Nickel2::Color(0, 0, 0));
    Nickel2::Renderer renderer(&window, "res/hdr_textures/kloppenheim_07_puresky_4k.hdr");
    Nickel2::Scene scene;

    FirstPersonCamera camera(&scene, &window, 90.0f, 0.1f, 0.01f, 1000.0f);
    camera.addComponent<Nickel2::ListenerComponent>(camera.getComponent<Nickel2::CameraComponent>());
    scene.setPrimaryCamera(&camera);

    Nickel2::Entity plagueMask = scene.createEntity("plagueMask");
    plagueMask.addComponent<Nickel2::MeshComponent>("res/plague_mask/scene.gltf");

    Nickel2::Entity music(&scene);
    music.addComponent<Nickel2::SourceComponent>(camera.getComponent<Nickel2::ListenerComponent>(), "res/music.wav", true);
    music.getComponent<Nickel2::SourceComponent>()->play();

    Nickel2::Scene* currentScene = &scene;

    while (!window.shouldClose()) {
        context.pollEvents();
        context.makeCurrent();
        window.update();

        camera.getComponent<Nickel2::CameraComponent>()->yaw += 2.0f * window.deltaTime;
        currentScene->update(window.deltaTime);

        if (window.input->getKey(NICKEL2_KEY_R))
            renderer.reloadShaders();

        renderer.imGuiNewFrame();

        ImGui::Begin("Nickel2");
        ImGui::Text("Hello, world!");
        ImGui::End();

        window.clear();
        renderer.render(currentScene, true, true, true);
        window.swapBuffers();
    }

    scene.destroy();
    renderer.destroy();
    window.destroy();
    context.destroy();
    return 0;
}
```