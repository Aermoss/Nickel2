#include <nickel2/window.hpp>

namespace nickel2 {
    void glfwErrorCallback(int errorCode, const char* description) {
        getContext()->logger->log(NICKEL2_ERROR, ("glfw: code: " + std::to_string(errorCode) + ", description: " + description + ".").c_str());
    }

    Window::Window(int32_t width, int32_t height, const char* title, const Color& backgroundColor, bool vsync, bool fullscreen, bool icon)
        : title(title), width(width), height(height), backgroundColor(backgroundColor) {
        Context* context = getContext();
        id = context->registerWindow(this);
        glfwWindowHint(GLFW_SAMPLES, 1);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwSetErrorCallback(glfwErrorCallback);
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
            context->vendorName = std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
            context->rendererName = std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
            glGetIntegerv(GL_NUM_EXTENSIONS, &context->extensionCount);

            context->logger->log(NICKEL2_INFO, (
                std::string("found renderer device:\n") + \
                "               > vendor: " + context->vendorName + "\n" + \
                "               > renderer: " + context->rendererName + "\n" + \
                "               > version: " + context->glVersion + "\n" + \
                "               > glsl: " + context->glslVersion).c_str()
            );

            context->logger->log(NICKEL2_INFO, ("supported extension count: " + std::to_string(context->extensionCount)).c_str());
        }
    }

    Window::~Window() {

    }

    void Window::update() {
        glfwMakeContextCurrent(window);

        int32_t width, height;
        glfwGetWindowSize(window, &width, &height);

        if (width != this->width || height != this->height) {
            this->width = width, this->height = height;
            glViewport(0, 0, width, height);
        }

        currentTime = glfwGetTime();

        if (lastTime == 0.0f)
            lastTime = currentTime;
            
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        if (events.find("update") != events.end())
            events["update"]();
    }

    void Window::clear() {
        glClearColor(backgroundColor.r / 255.0f, backgroundColor.g / 255.0f, backgroundColor.b / 255.0f, backgroundColor.a / 255.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::setEvent(std::string name, windowEvent_t func) {
        events[name] = func;
    }

    void Window::setVSync(bool vsync) {
        glfwSwapInterval(vsync ? 1 : 0);
    }

    void Window::setTitle(const char* title) {
        glfwSetWindowTitle(window, title);
        this->title = title;
    }

    void Window::getSize(int32_t* width, int32_t* height) {
        glfwGetWindowSize(window, width, height);
    }

    GLFWwindow* Window::getGLFWWindow() {
        return window;
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