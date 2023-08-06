# Nickel2
A game engine written in C++ using OpenGL.

# Examples
## Creating a window
```c++
#include "nickel2.hpp"

int main() {
    nickel2::Context context;
    nickel2::Window window(1200, 600, "Nickel2", nickel2::Color(0, 64, 64), true);

    nickel2::Renderer renderer(&window);
    nickel2::FPSCamera camera(&window, glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, 100.0f, 0.01f, 1000.0f);

    renderer.lights = {
        nickel2::Light(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f), 3.0f)
    };

    while (!window.shouldClose()) {
        context.pollEvents();
        window.update();
        window.clear();
        camera.processInputs();
        renderer.render(&camera);
        window.swapBuffers();
    }

    camera.destroy();
    renderer.destroy();
    window.destroy();
    context.destroy();
    return 0;
}
```
