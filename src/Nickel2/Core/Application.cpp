#include <Nickel2/nkpch.hpp>
#include <Nickel2/Core/Application.hpp>
#include <Nickel2/Core/Input.hpp>
#include <Nickel2/Core/Window.hpp>
#include <Nickel2/Renderer/Renderer.hpp>

#include <filesystem>

namespace Nickel2 {
    Application* Application::instance = nullptr;

    Application::Application(const ApplicationSpecification& specification) : specification(specification) {
        this->instance = this;

        if (!specification.workingDirectory.empty())
            std::filesystem::current_path(specification.workingDirectory);

        window = std::unique_ptr<Window>(Window::Create({
            .title = specification.name, .iconPath = specification.iconPath,
            .width = specification.width, .height = specification.height, .x = -1, .y = -1,
            .decorated = specification.decorated, .fullscreen = specification.fullscreen,
            .maximized = specification.maximized, .darkMode = specification.darkMode,
            .vSync = specification.vSync, .resizable = specification.resizable
        }));

        window->Initialize();
        window->SetEventCallback([this](Event& event) { HandleEvent(event); });
        if (specification.maximized) window->Maximize();

        Renderer::Initialize();
        Input::Initialize(static_cast<GLFWwindow*>(window->GetHandle()));
        
        if (specification.renderer)
            renderer = std::make_unique<SceneRenderer>(window.get());

        imGuiLayer = ImGuiLayer::Create();
        PushLayer(imGuiLayer);

        if (!alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT"))
            Nickel2::Logger::Log(Nickel2::Logger::Level::Error, "OpenAL", "ALC enumeration extension not available.");

        const char* defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
        ALCdevice* device = alcOpenDevice(defaultDeviceName);

        if (!device) {
            Nickel2::Logger::Log(Nickel2::Logger::Level::Error, "OpenAL", ("Unable to open default audio device: \"" + std::string(defaultDeviceName) + "\".").c_str());
        } else {
            Nickel2::Logger::Log(Nickel2::Logger::Level::Info, "OpenAL", ("Found audio device: \"" + std::string(defaultDeviceName) + "\".").c_str());
            alcContext = alcCreateContext(device, NULL);

            if (!alcMakeContextCurrent(alcContext))
                Nickel2::Logger::Log(Nickel2::Logger::Level::Error, "OpenAL", "Failed to make ALC context current.");
        }

    }

    Application::~Application() {
        alcCloseDevice(alcGetContextsDevice(alcContext));
        alcDestroyContext(alcContext);
        alcMakeContextCurrent(nullptr);

        for (Layer* layer : layerStack) {
            layer->OnDetach();
            delete layer;
        }

        Input::Terminate();
        Renderer::Terminate();

        window->SetEventCallback([](Event& event) {});
        window->Terminate();

        this->instance = nullptr;
    }

    void Application::PushLayer(Layer* layer) {
        layerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PopLayer(Layer* layer) {
        layerStack.PopLayer(layer);
        layer->OnDetach();
    }

    void Application::Initialize() {
        this->OnInitialization();

        float deltaTime = 0.0f, \
            lastTime = glfwGetTime();

        while (!this->isTerminated) {
            window->ProcessEvents();

            float currentTime = glfwGetTime();
            deltaTime = currentTime - lastTime, lastTime = currentTime;
            deltaTime *= this->timeScale;

            if (!this->isMinimized) {
                RenderCommand::Clear();
                this->OnUpdate(deltaTime);

                for (Layer* layer : this->layerStack)
                    layer->OnUpdate(deltaTime);

                imGuiLayer->Begin();

                for (Layer* layer : this->layerStack)
                    layer->OnRender(deltaTime);

                if (specification.renderer && currentScene != nullptr)
                    renderer->Render(currentScene, deltaTime);

                imGuiLayer->End();
                window->SwapBuffers();
            }
        }
    }

    void Application::Terminate() {
        if (this->isTerminated) return;
        this->isTerminated = true;
        this->OnTermination();
    }

    void Application::HandleEvent(Event& event) {
        Input::HandleEvent(event);

        switch (event.GetEventType()) {
            case EventType::WindowResize: {
                WindowResizeEvent& resizeEvent = static_cast<WindowResizeEvent&>(event);
                glm::vec2 size = resizeEvent.GetSize();
                if (size.x == 0 || size.y == 0) break;
                RenderCommand::SetViewport(0, 0, size.x, size.y);
                break;
            } case EventType::WindowMinimize: {
                this->isMinimized = static_cast<WindowMinimizeEvent&>(event).IsMinimized();
                break;
            } case EventType::WindowClose: {
                this->Terminate();
                break;
            } default: {
                break;
            }
        }

        this->OnEvent(event);

        for (Layer* layer : this->layerStack)
            layer->OnEvent(event);
    }
}