#pragma once

#include "Base.hpp"
#include "Window.hpp"
#include "LayerStack.hpp"
#include "../Event/WindowEvent.hpp"
#include "../Renderer/ImGuiLayer.hpp"
#include "../Renderer/SceneRenderer.hpp"

#include <filesystem>

namespace Nickel2 {
    struct ApplicationSpecification {
        std::string name = "Nickel2";
        std::string workingDirectory;
        std::filesystem::path iconPath;
        int32_t width = 1600, height = 900;
        bool decorated = true, fullscreen = false, maximized = false;
        bool darkMode = true, vSync = true, resizable = true, renderer = true;
    };

    class Application {
        using EventCallbackFunc = std::function<void(Event&)>;

        private:
            std::unique_ptr<Window> window;
            std::unique_ptr<SceneRenderer> renderer;
            Scene* currentScene;

            ApplicationSpecification specification;
            bool isTerminated = false, isMinimized = false;
            ALCcontext* alcContext;

            LayerStack layerStack;
            ImGuiLayer* imGuiLayer;
            static Application* instance;
            float timeScale = 1.0f;

        public:
            Application(const ApplicationSpecification& specification);
            virtual ~Application();

            virtual void OnInitialization() {}
            virtual void OnTermination() {}
            virtual void OnUpdate(float deltaTime) {}
            virtual void OnRender(float deltaTime) {}
            virtual void OnEvent(Event& event) {}

            void PushLayer(Layer* layer);
            void PopLayer(Layer* layer);

            void Initialize();
            void Terminate();

            void HandleEvent(Event& event);

            void SetTimeScale(float timeScale) { this->timeScale = timeScale; }
            float GetTimeScale() const { return timeScale; }

            inline Window& GetWindow() const { return *window; }
            inline SceneRenderer& GetRenderer() const { return *renderer; }
            inline Scene& GetScene() { return *currentScene; }
            inline void SetScene(Scene* scene) { currentScene = scene; }
            static inline Application& Get() { return *instance; }
            const ApplicationSpecification& GetSpecification() const { return specification; }
    };

    Application* CreateApplication(int32_t argc, const uint8_t* argv[]);
}