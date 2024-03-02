#include <Nickel2/nkpch.hpp>
#include <Nickel2/Core/Window.hpp>

namespace Nickel2 {
    void glfwErrorCallback(int errorCode, const char* description) {
        getContext()->logger->log(NICKEL2_ERROR, ("glfw: code: " + std::to_string(errorCode) + ", description: " + description + ".").c_str());
    }

    Window::Window(int32_t width, int32_t height, const char* title, const Color& backgroundColor, bool vsync, bool fullscreen, bool icon, bool darkMode)
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

        int32_t useDarkMode = static_cast<int32_t>(darkMode);
        DwmSetWindowAttribute(glfwGetWin32Window(window), 20, &useDarkMode, sizeof(useDarkMode));

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
            glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &context->maxTessLevel);
            glGetIntegerv(GL_NUM_EXTENSIONS, &context->extensionCount);

            context->logger->log(NICKEL2_INFO, (
                std::string("found renderer device:\n") + \
                "               > vendor: " + context->vendorName + "\n" + \
                "               > renderer: " + context->rendererName + "\n" + \
                "               > version: " + context->glVersion + "\n" + \
                "               > glsl: " + context->glslVersion).c_str()
            );

            context->logger->log(NICKEL2_INFO, ("supported extension count: " + std::to_string(context->extensionCount)).c_str());
            context->logger->log(NICKEL2_INFO, ("maximum allowed tessellation level: " + std::to_string(context->maxTessLevel)).c_str());
        }

        imguiContext = ImGui::CreateContext();
        ImGui::SetCurrentContext(imguiContext);

        ImGuiIO& io = ImGui::GetIO(); (void) io;
        io.IniFilename = nullptr, io.LogFilename = nullptr;
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.63f);
        style.Colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 0.31f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.63f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.23f, 0.23f, 0.23f, 0.63f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.19f, 0.19f, 0.19f, 0.39f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.63f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.35f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.63f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.63f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.38f, 0.38f, 0.38f, 0.63f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.22f, 0.55f, 0.74f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.00f, 0.00f, 0.00f, 0.63f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.20f, 0.22f, 0.23f, 0.63f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.19f, 0.19f, 0.19f, 0.39f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.20f, 0.78f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.29f, 0.29f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.19f, 0.19f, 0.19f, 0.15f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
        style.Colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        style.Colors[ImGuiCol_TabHovered] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
        style.Colors[ImGuiCol_TabActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        style.Colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
        style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
        style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

        style.WindowPadding = ImVec2(6.0f, 6.0f);
        style.FramePadding = ImVec2(5.0f, 2.0f);
        style.CellPadding = ImVec2(6.0f, 6.0f);
        style.ItemSpacing = ImVec2(6.0f, 6.0f);
        style.ItemInnerSpacing = ImVec2(6.0f, 6.0f);
        style.TouchExtraPadding = ImVec2(0.0f, 0.0f);
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
        style.IndentSpacing = 25.0f;
        style.ScrollbarSize = 10.0f;
        style.GrabMinSize = 10.0f;
        style.WindowBorderSize = 1.0f;
        style.ChildBorderSize = 1.0f;
        style.PopupBorderSize = 1.0f;
        style.FrameBorderSize = 1.0f;
        style.TabBorderSize = 1.0f;
        style.WindowRounding = 5.0f;
        style.ChildRounding = 4.0f;
        style.FrameRounding = 2.0f;
        style.PopupRounding = 4.0f;
        style.ScrollbarRounding = 9.0f;
        style.GrabRounding = 2.0f;
        style.LogSliderDeadzone = 4.0f;
        style.TabRounding = 4.0f;

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    Window::~Window() {

    }

    void Window::update() {
        glfwMakeContextCurrent(window);
        ImGui::SetCurrentContext(imguiContext);
        context->makeWindowCurrent(id);

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

    HWND Window::getWindowHandle() {
        return glfwGetWin32Window(window);
    }

    GLFWwindow* Window::getGLFWWindow() {
        return window;
    }

    ImGuiContext* Window::getImGuiContext() {
        return imguiContext;
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
        ImGui::SetCurrentContext(imguiContext);
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext(imguiContext);

        Context* context = getContext();
        context->removeWindow(id);
        delete input;
        glfwDestroyWindow(window);
    }
}