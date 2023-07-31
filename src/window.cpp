#include "window.hpp"

namespace nickel2 {
    Window::Window(uint32_t width, uint32_t height, const char* title, bool vsync, bool fullscreen, bool icon) : title(title) {
        Context* context = getContext();
        id = context->registerWindow(this);
        glfwWindowHint(GLFW_SAMPLES, 1);
        glfwWindowHint(GLFW_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_VERSION_MINOR, 6);
        // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        window = glfwCreateWindow(width, height, title, fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
        if (window) { context->logger->log(NICKEL2_INFO, ("window named \"" + std::string(title) + "\" was created successfully, id: " + std::to_string(id) + ".").c_str());}
        else { context->logger->log(NICKEL2_FATAL_ERROR, ("an error occurred while creating the window named \"" + std::string(title) + "\", id: " + std::to_string(id) + ".").c_str()); }
        glfwMakeContextCurrent(window);
        glfwSwapInterval(vsync ? 1 : 0);
        gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        input = new InputHandler(window);
        lastTime = 0.0f;

        if (icon) {
            GLFWimage images[1]; 
            images[0].pixels = stbi_load("res/icon.png", &images[0].width, &images[0].height, 0, 4);
            glfwSetWindowIcon(window, 1, images); 
            stbi_image_free(images[0].pixels);
        }

        if (context->glVersion.empty()) {
            context->glVersion = std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
            context->glslVersion = std::string(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
            context->glExtensions = std::string(reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS)));
            context->vendorName = std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
            context->rendererName = std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));

            context->logger->log(NICKEL2_INFO, (
                std::string("found renderer device:\n") + \
                "            > vendor: " + context->vendorName + "\n" + \
                "            > renderer: " + context->rendererName + "\n" + \
                "            > version: " + context->glVersion + "\n" + \
                "            > glsl: " + context->glslVersion).c_str()
            );

            uint32_t count = context->glExtensions.empty() ? 0 : 1;

            for (uint32_t i = 0; i < context->glExtensions.size(); i++) {
                if (context->glExtensions[i] == ' ')
                    count++;
            }

            context->logger->log(NICKEL2_INFO, ("supported extension count: " + std::to_string(count)).c_str());
        }
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

    void Window::setTitle(const char* title) {
        glfwSetWindowTitle(window, title);
        this->title = title;
    }

    std::string Window::getTitle() {
        return title;
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