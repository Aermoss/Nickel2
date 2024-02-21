# Nickel2
A game engine written in C++ using OpenGL.

# Work In Progress
This project is currently under development.

# Examples
## Creating a window
```c++
#include <nickel2/nickel2.hpp>

int main(int argc, const char* argv[]) {
    nickel2::Context context;
    nickel2::Window window(1200, 600, "Nickel2", nickel2::Color(0, 0, 0), true);
    nickel2::Renderer renderer(&window);
    nickel2::FirstPersonCamera camera(&window, 90.0f, 100.0f, 0.01f, 1000.0f);
    nickel2::audio::Listener listener(&camera);
    nickel2::Scene scene;

    nickel2::Scene* currentScene = &scene;
    nickel2::Camera* currentCamera = &camera;

    while (!window.shouldClose()) {
        context.makeCurrent();
        context.pollEvents();
        camera.processInputs();
        listener.update();
        window.update();
        window.clear();

        if (window.input->getKey(NICKEL2_KEY_R))
            renderer.reloadShaders();

        renderer.render(currentCamera, currentScene);
        window.swapBuffers();
    }

    scene.destroy();
    listener.destroy();
    camera.destroy();
    renderer.destroy();
    window.destroy();
    context.destroy();
    return 0;
}
```
