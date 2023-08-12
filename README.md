# Nickel2
A game engine written in C++ using OpenGL.

# Examples
## Creating a window
```c++
#include <nickel2/nickel2.hpp>

int main() {
    nickel2::Context context;
    nickel2::Window window(1200, 600, "Nickel2", nickel2::Color(0, 0, 0), true);
    
    nickel2::Renderer renderer(&window);
    nickel2::FirstPersonCamera camera(&window, 90.0f, 100.0f, 0.01f, 1000.0f);
    nickel2::Scene scene;

    while (!window.shouldClose()) {
        context.pollEvents();
        camera.processInputs();
        window.update();
        window.clear();
        renderer.render(&camera, &scene);
        window.swapBuffers();
    }

    scene.destroy();
    camera.destroy();
    renderer.destroy();
    window.destroy();
    context.destroy();
    return 0;
}
```
