#include "window.hpp"

namespace nickel2 {
    Window::Window(uint32_t width, uint32_t height, const char* title, bool vsync) {
        Context* context = getContext();
        id = context->registerWindow(this);
        glfwWindowHint(GLFW_SAMPLES, 1);
        glfwWindowHint(GLFW_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_VERSION_MINOR, 6);
        // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        glfwMakeContextCurrent(window);
        glfwSwapInterval(vsync ? 1 : 0);
        gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        input = new InputHandler(window);
        lastTime = 0.0f;
    }

    Window::~Window() {

    }

    void Window::update() {
        if (events.find("update") != events.end())
            events["update"]();

        currentTime = glfwGetTime();

        if (lastTime == 0.0f)
            lastTime = currentTime;
            
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
    }

    void Window::clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::setEvent(std::string name, windowEvent_t func) {
        events[name] = func;
    }

    void Window::getSize(int32_t* width, int32_t* height) {
        glfwGetWindowSize(window, width, height);
    }

    bool Window::shouldClose() {
        return glfwWindowShouldClose(window);
    }

    void Window::swapBuffers() {
        glfwSwapBuffers(window);
    }

    void Window::destroy() {
        Context* context = getContext();
        context->removeWindow(id);
        delete input;
        glfwDestroyWindow(window);
    }
}