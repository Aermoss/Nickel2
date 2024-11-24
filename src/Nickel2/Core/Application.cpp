#include <Nickel2/nkpch.hpp>
#include <Nickel2/Core/Application.hpp>
#include <Nickel2/Core/Input.hpp>
#include <Nickel2/Core/Window.hpp>
#include <Nickel2/Renderer/Renderer.hpp>
#include <Nickel2/Physics/PhysicsSystem.hpp>
#include <Nickel2/Physics/PhysicsLayer.hpp>

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
        AudioSystem::Initialize();
        PhysicsSystem::Initialize();
        PhysicsLayerManager::ClearLayers();
		PhysicsLayerManager::AddLayer("Default");
        Input::Initialize(static_cast<GLFWwindow*>(window->GetHandle()));
        
        if (specification.renderer)
            renderer = std::make_unique<SceneRenderer>(window.get());

        imGuiLayer = ImGuiLayer::Create();
        PushLayer(imGuiLayer);
    }

    Application::~Application() {
        for (Layer* layer : layerStack) {
            layer->OnDetach();
            delete layer;
        }

        Input::Terminate();
        PhysicsLayerManager::ClearLayers();
        PhysicsSystem::Terminate();
        AudioSystem::Terminate();
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
                AudioSystem::Update(deltaTime);
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
                Renderer::OnWindowResize(size.x, size.y);
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