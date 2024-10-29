#include <Nickel2/nkpch.hpp>
#include <Nickel2/Core/Logger.hpp>
#include <Nickel2/Core/Window.hpp>
#include <Nickel2/Core/Application.hpp>
#include <Nickel2/Core/Input.hpp>
#include <Nickel2/Renderer/RenderCommand.hpp>
#include <Nickel2/Event/WindowEvent.hpp>
#include <Nickel2/Event/KeyEvent.hpp>
#include <Nickel2/Event/MouseEvent.hpp>

#include <imgui.h>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

namespace Nickel2 {
    static void GLFWErrorCallback(int error, const char* description) {
        Nickel2::Logger::Log(Logger::Level::Error, "GLFW", (description + std::string(".")).c_str());
    }

    void Window::Initialize() {
        int32_t result = glfwInit();
        NK_CORE_ASSERT(result, "Couldn't intialize GLFW.");
        glfwSetErrorCallback(GLFWErrorCallback);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
#if defined(NK_DEBUG)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, specification.resizable);
        glfwWindowHint(GLFW_DECORATED, specification.decorated);

        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

        Nickel2::Logger::Log(Nickel2::Logger::Level::Info, "Window", ("Found display device: \"" + std::string(glfwGetMonitorName(primaryMonitor)) + "\".").c_str());

        if (specification.fullscreen) {
            specification.width = mode->width, specification.height = mode->height;
            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        }

        window = glfwCreateWindow(specification.width, specification.height, \
            specification.title.c_str(), specification.fullscreen ? primaryMonitor : nullptr, nullptr);
        
        if (specification.maximized) { this->Maximize(); }
        else {
            if (specification.x < 0 || specification.y < 0) this->Center();
            else this->SetPosition({ (float) specification.x, (float) specification.y });
        }

        if (!specification.iconPath.empty()) {
            GLFWimage icon; int32_t channels;
            std::string iconPathStr = specification.iconPath.string();
            icon.pixels = stbi_load(iconPathStr.c_str(), &icon.width, &icon.height, &channels, 4);
            glfwSetWindowIcon(window, 1, &icon);
            stbi_image_free(icon.pixels);
        }

        int32_t useDarkMode = static_cast<int32_t>(specification.darkMode);
        DwmSetWindowAttribute(GetHWND(), 20, &useDarkMode, sizeof(useDarkMode));
        specification.darkMode = static_cast<bool>(useDarkMode);

        rendererContext = RendererContext::Create(window);
        rendererContext->Initialize();

        glfwSetWindowUserPointer(window, this);

        bool isRawMouseMotionSupported = glfwRawMouseMotionSupported();
        if (isRawMouseMotionSupported) glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        else Logger::Log(Logger::Level::Warn, "Input", "Raw mouse motion not supported.");

        glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
            Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
            WindowCloseEvent event;
            instance->GetEventCallback()(event);
        });

        glfwSetWindowIconifyCallback(window, [](GLFWwindow* window, int32_t iconified) {
            Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
            WindowMinimizeEvent event((bool) iconified);
            instance->GetEventCallback()(event);
        });

        glfwSetWindowMaximizeCallback(window, [](GLFWwindow* window, int32_t maximized) {
            Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
            WindowMaximizeEvent event((bool) maximized);
            instance->GetEventCallback()(event);
        });

        glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int32_t width, int32_t height) {
            Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
            instance->specification.width = width, instance->specification.height = height;
            WindowResizeEvent event((uint32_t) width, (uint32_t) height);
            instance->GetEventCallback()(event);
        });

        glfwSetWindowPosCallback(window, [](GLFWwindow* window, int32_t x, int32_t y) {
            Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
            WindowMoveEvent event((uint32_t) x, (uint32_t) y);
            instance->GetEventCallback()(event);
        });

        glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int32_t focused) {
            Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
            WindowFocusEvent event((bool) focused);
            instance->GetEventCallback()(event);
        });

        glfwSetKeyCallback(window, [](GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
            Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));

            switch (action) {
                case GLFW_PRESS: {
                    KeyPressEvent event((KeyCode) key);
                    instance->GetEventCallback()(event);
                    break;
                } case GLFW_RELEASE: {
                    KeyReleaseEvent event((KeyCode) key);
                    instance->GetEventCallback()(event);
                    break;
                } case GLFW_REPEAT: {
                    KeyHoldEvent event((KeyCode) key);
                    instance->GetEventCallback()(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(window, [](GLFWwindow* window, uint32_t codepoint) {
            Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
            KeyTypeEvent event((KeyCode) codepoint);
            instance->GetEventCallback()(event);
        });

        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
            Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
            MouseMoveEvent event((float) x, (float) y);
            instance->GetEventCallback()(event);
        });

        glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset) {
            Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
            MouseScrollEvent event((float) xOffset, (float) yOffset);
            instance->GetEventCallback()(event);
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int32_t button, int32_t action, int32_t mods) {
            Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));

            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressEvent event((MouseButton) button);
                    instance->GetEventCallback()(event);
                    break;
                } case GLFW_RELEASE: {
                    MouseButtonReleaseEvent event((MouseButton) button);
                    instance->GetEventCallback()(event);
                    break;
                }
            }
        });
    }

    void Window::Terminate() {
        if (!this->isTerminated) return;
        this->isTerminated = true;
        rendererContext->Terminate();
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Window::ProcessEvents() {
        rendererContext->CheckErrors();
        glfwPollEvents();
    }

    void Window::SwapBuffers() {
        rendererContext->SwapBuffers();
    }

    void Window::Maximize() {
        glfwMaximizeWindow(window);
    }

    void Window::Center() {
        GLFWvidmode* mode = (GLFWvidmode*) glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowPos(window, (int32_t) ((mode->width - specification.width) / 2), (int32_t) ((mode->height - specification.height) / 2));
    }

    void Window::SetSize(const glm::vec2& size) {
        glfwSetWindowSize(window, (int32_t) size.x, (int32_t) size.y);
    }

    glm::vec2 Window::GetSize() {
        glfwGetWindowSize(window, &specification.width, &specification.height);
        return { (float) specification.width, (float) specification.height };
    }

    void Window::SetPosition(const glm::vec2& position) {
        glfwSetWindowPos(window, (int32_t) position.x, (int32_t) position.y);
    }

    glm::vec2 Window::GetPosition() {
        glfwGetWindowPos(window, &specification.x, &specification.y);
        return { (float) specification.x, (float) specification.y };
    }

    void Window::SetTitle(const std::string& title) {
        specification.title = title;
        glfwSetWindowTitle(window, specification.title.c_str());
    }

    const std::string Window::GetTitle() {
        return glfwGetWindowTitle(window);
    }

    void Window::SetEventCallback(const EventCallbackFunc& callback) {
        this->eventCallback = callback;
    }

    Window::EventCallbackFunc Window::GetEventCallback() {
        return this->eventCallback;
    }

    void Window::SetVSync(bool enabled) {
        specification.vSync = enabled;
        glfwSwapInterval(specification.vSync ? 1 : 0);
    }

    bool Window::GetVSync() {
        return specification.vSync;
    }

    Window* Window::Create(const WindowSpecification& specification) {
        return new Window(specification);
    }
}