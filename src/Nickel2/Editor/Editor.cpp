#include <Nickel2/nkpch.hpp>
#include <Nickel2/Nickel2.hpp>

namespace Nickel2 {
    class FirstPersonCamera : public Entity {
        private:
            Window* window;

        public:
            float sensitivity, targetFov, lastScroll = 0.0f, targetSpeed = 0.0f, \
                currentSpeed = 0.0f, normalSpeed = 0.125f, sprintSpeed = 0.250f;

            double realYaw = 0.0, realPitch = 0.0, \
                interpolatedYaw = 0.0, interpolatedPitch = 0.0;

            float lastAngleUpdate = 0.0f, angleRange = 2.0f, offsetMultiplier = 3.0f;
            glm::vec2 randomAngle = glm::vec2(0.0f), currentAngle = glm::vec2(0.0f), interpolatedAngle = glm::vec2(0.0f);
            glm::vec3 targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);

            FirstPersonCamera(Scene* scene, Window* window, float fov, float sensitivity, float nearPlane, float farPlane)
                : Entity(scene), window(window), sensitivity(sensitivity), targetFov(fov) {
                AddComponent<CameraComponent>(window, fov, nearPlane, farPlane, NK_EULER_OVERRIDE);
            }

            ~FirstPersonCamera() {}

            void OnCreate() override {}
            void OnDestroy() override {}

            void OnUpdate(float deltaTime) override {
                if (Input::IsKeyReleased(Key::LeftAlt)) {
                    Input::SetCursorMode(CursorMode::Hidden);
                } else {
                    Input::SetCursorMode(CursorMode::Normal);
                    return;
                }

                if (!glfwGetWindowAttrib((GLFWwindow*) window->GetHandle(), GLFW_FOCUSED)) return;
                currentSpeed = glm::lerp(currentSpeed, Input::IsKeyHeld(Key::LeftControl) ? sprintSpeed : normalSpeed, 7.5f * deltaTime);
                TransformComponent& transform = GetComponent<TransformComponent>();
                Camera* camera = GetComponent<CameraComponent>().camera;

                if (Input::IsKeyHeld(Key::W))
                    targetPosition += currentSpeed * glm::vec3(glm::cos(glm::radians(realYaw)), 0.0f, glm::sin(glm::radians(realYaw))) * deltaTime * 10.0f;
                
                if (Input::IsKeyHeld(Key::S))
                    targetPosition += currentSpeed * -glm::vec3(glm::cos(glm::radians(realYaw)), 0.0f, glm::sin(glm::radians(realYaw))) * deltaTime * 10.0f;

                if (Input::IsKeyHeld(Key::A))
                    targetPosition += currentSpeed * -glm::normalize(glm::cross(camera->front, camera->up)) * deltaTime * 10.0f;

                if (Input::IsKeyHeld(Key::D))
                    targetPosition += currentSpeed * glm::normalize(glm::cross(camera->front, camera->up)) * deltaTime * 10.0f;

                if (Input::IsKeyHeld(Key::Space))
                    targetPosition += currentSpeed * camera->up * deltaTime * 10.0f;

                if (Input::IsKeyHeld(Key::LeftShift))
                    targetPosition += currentSpeed * -camera->up * deltaTime * 10.0f;

                transform.SetTranslation(glm::lerp(transform.GetTranslation(), targetPosition, 7.5f * deltaTime));
                glm::vec2 position = Input::GetMousePosition(), size = window->GetSize();

                double xOffset = sensitivity * (position.x - (int32_t) (size.x / 2)) / size.x;
                double yOffset = sensitivity * (position.y - (int32_t) (size.y / 2)) / size.y;

                if (lastAngleUpdate + 0.5f < window->GetTime()) {
                    lastAngleUpdate = window->GetTime();
                    randomAngle = glm::linearRand(glm::vec2(glm::radians(-angleRange)), glm::vec2(glm::radians(angleRange)));
                }
                
                interpolatedAngle = glm::lerp(interpolatedAngle, randomAngle, 1.0f * deltaTime);
                currentAngle += interpolatedAngle * 250.0f * deltaTime;

                if (currentAngle.x > glm::pi<float>() * 2.0f) currentAngle.x -= glm::pi<float>() * 2.0f;
                if (currentAngle.x < glm::pi<float>() * 2.0f) currentAngle.x += glm::pi<float>() * 2.0f;

                if (currentAngle.y > glm::pi<float>() * 2.0f) currentAngle.y -= glm::pi<float>() * 2.0f;
                if (currentAngle.y < glm::pi<float>() * 2.0f) currentAngle.y += glm::pi<float>() * 2.0f;

                realYaw += xOffset, realPitch = glm::clamp(realPitch - yOffset, -89.99, 89.99);
                interpolatedPitch = glm::lerp(interpolatedPitch, realPitch, static_cast<double>(15.0f * deltaTime));
                interpolatedYaw = glm::lerp(interpolatedYaw, realYaw, static_cast<double>(15.0f * deltaTime));
                camera->pitch = glm::clamp(interpolatedPitch + glm::sin(currentAngle.y) * offsetMultiplier, -89.99, 89.99);
                camera->yaw = interpolatedYaw + glm::cos(currentAngle.x) * offsetMultiplier;

                float change = lastScroll - Input::GetMouseScroll().y;
                lastScroll = Input::GetMouseScroll().y;

                if (Input::IsKeyHeld(Key::F)) {
                    Application::Get().SetTimeScale(glm::clamp(Application::Get().GetTimeScale() + change * 0.1f, 0.1f, 1.0f));
                } else {
                    targetFov = glm::clamp(targetFov + change * 5.0f, 10.0f, 110.0f);
                    camera->fov = glm::lerp(camera->fov, targetFov + (currentSpeed - normalSpeed) / sprintSpeed * 20.0f, 7.5f * deltaTime);
                    sensitivity = camera->fov * 1.5f + 10.0f;
                } Input::SetMousePosition(size / 2.0f);
            }
    };

    class EditorLayer : public Layer {
        private:
            float yaw = 45.0f, pitch = 45.0f;
            int32_t gizmoType = ImGuizmo::TRANSLATE;

            std::shared_ptr<FirstPersonCamera> camera;
            std::shared_ptr<Entity> sun, airplane, currentEntity;
            std::shared_ptr<Scene> scene;

        public:
            EditorLayer() {
                Logger::Log(Logger::Level::Info, "EditorLayer", "Editor layer created.");
            }

            virtual ~EditorLayer() override {
                Logger::Log(Logger::Level::Info, "EditorLayer", "Editor layer destroyed.");
            }

            virtual void OnAttach() override {
                Logger::Log(Logger::Level::Info, "EditorLayer", "Editor layer attached.");
                std::chrono::time_point<std::chrono::high_resolution_clock> startTime = std::chrono::high_resolution_clock::now();

                Window& window = Application::Get().GetWindow();
                Application::Get().GetRenderer().LoadSkybox("res/hdr_textures/evening_road_01_puresky_4k.hdr");

                scene = std::make_shared<Scene>();
                Application::Get().SetScene(scene.get());

                camera = std::make_shared<FirstPersonCamera>(scene.get(), &window, 90.0f, 0.1f, 0.1f, 5000.0f);
                camera->AddComponent<ListenerComponent>(DistanceModel::InverseDistanceClamped);
                scene->SetPrimaryCamera(camera.get());

                sun = std::make_shared<Entity>(scene.get(), "sun");
                sun->AddComponent<DirectionalLightComponent>();

                std::chrono::time_point<std::chrono::high_resolution_clock> loadingStartTime = std::chrono::high_resolution_clock::now();

                airplane = std::make_shared<Entity>(scene.get(), "airplane");
                airplane->AddComponent<MeshComponent>("res/su_27/scene.gltf");
                airplane->GetComponent<TransformComponent>().SetScale(glm::vec3(0.01f));

                currentEntity = airplane;

                std::chrono::time_point<std::chrono::high_resolution_clock> loadingEndTime = std::chrono::high_resolution_clock::now();
                Logger::Log(Logger::Level::Warn, ("Loading time: " + std::to_string(static_cast<std::chrono::duration<float>>(loadingEndTime - loadingStartTime).count())).c_str());

                std::chrono::time_point<std::chrono::high_resolution_clock> endTime = std::chrono::high_resolution_clock::now();
                Logger::Log(Logger::Level::Warn, ("Launch time: " + std::to_string(static_cast<std::chrono::duration<float>>(endTime - startTime).count())).c_str());
            }

            virtual void OnDetach() override {
                Logger::Log(Logger::Level::Info, "EditorLayer", "Editor layer detached.");
            }

            virtual void OnUpdate(float deltaTime) override {
                SceneRenderer& renderer = Application::Get().GetRenderer();
                scene->Update(deltaTime);

                const float turnSpeed = 10.0f;
                if (Input::IsKeyHeld(Key::Left)) yaw -= turnSpeed * deltaTime;
                if (Input::IsKeyHeld(Key::Right)) yaw += turnSpeed * deltaTime;
                if (Input::IsKeyHeld(Key::Up)) pitch -= turnSpeed * deltaTime;
                if (Input::IsKeyHeld(Key::Down)) pitch += turnSpeed * deltaTime;

                sun->GetComponent<DirectionalLightComponent>().direction = \
                    glm::normalize(glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw)) * cos(glm::radians(pitch))));

                if (Input::IsKeyHeld(Key::R))
                    renderer.ReloadShaders();

                if (Input::IsKeyHeld(Key::Q))
                    renderer.LoadSkybox("res/hdr_textures/evening_road_01_puresky_4k.hdr");
                
                if (Input::IsKeyHeld(Key::E))
                    renderer.LoadSkybox("res/hdr_textures/kloppenheim_07_puresky_4k.hdr");

                if (Input::IsKeyHeld(Key::D1)) gizmoType = ImGuizmo::TRANSLATE;
                if (Input::IsKeyHeld(Key::D2)) gizmoType = ImGuizmo::ROTATE;
                if (Input::IsKeyHeld(Key::D3)) gizmoType = ImGuizmo::SCALE;
            }

            virtual void OnRender(float deltaTime) override {
                SceneRenderer& renderer = Application::Get().GetRenderer();
                Window& window = Application::Get().GetWindow();
                glm::vec2 size = window.GetSize();

                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::Begin("Viewport", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize);
                ImGui::SetWindowPos(ImVec2(0.0f, 0.0f)), ImGui::SetWindowSize(ImVec2(size.x, size.y));
                
                ImVec2 viewportMinRegion = ImGui::GetWindowContentRegionMin(), \
                    viewportMaxRegion = ImGui::GetWindowContentRegionMax(), viewportOffset = ImGui::GetWindowPos();

                glm::vec2 viewportBounds[] = {
                    { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y },
                    { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y }
                };

                ImGuizmo::SetDrawlist();
                ImGuizmo::SetRect(viewportBounds[0].x, viewportBounds[0].y, viewportBounds[1].x - viewportBounds[0].x, viewportBounds[1].y - viewportBounds[0].y);

                const glm::mat4& cameraProjection = camera->GetComponent<CameraComponent>()->GetProjection();
                glm::mat4 cameraView = camera->GetComponent<CameraComponent>()->GetView();
                glm::mat4 transform = currentEntity->GetComponent<TransformComponent>().GetWorldTransform();
                float snapValues[] = { 0.1f, 1.0f, 0.1f };

                ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), \
                    (ImGuizmo::OPERATION) gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), \
                        nullptr, Input::IsKeyHeld(Key::LeftShift) ? snapValues : nullptr);

                if (ImGuizmo::IsUsing()) {
                    glm::vec3 translation, scale; glm::quat rotation;
                    Math::DecomposeTransform(transform, translation, rotation, scale);
                    currentEntity->GetComponent<TransformComponent>().SetTranslation(translation);
                    currentEntity->GetComponent<TransformComponent>().SetRotation(rotation);
                    currentEntity->GetComponent<TransformComponent>().SetScale(scale);
                }

                ImGui::End();
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();
                ImGui::PopStyleVar();

                renderer.postProcessingShader->Bind();
                renderer.postProcessingShader->SetFloat("vignetteStrength", 0.2f);
                renderer.postProcessingShader->SetFloat4("vignetteColor", glm::vec4(0.0f));
                renderer.postProcessingShader->Unbind();
            }

            virtual void OnEvent(Event& event) override {
                if (ImGui::GetIO().WantCaptureMouse)
                    return;
            }
    };

    class EditorApplication : public Application {
        private:
            EditorLayer* layer;

        public:
            EditorApplication(const ApplicationSpecification& specification) : Application(specification) {
                Logger::Log(Logger::Level::Info, "Editor", "Application created.");
                layer = new EditorLayer();
            }

            ~EditorApplication() {
                delete layer;
                Logger::Log(Logger::Level::Info, "Editor", "Application destroyed.");
            }

            void OnInitialization() override {
                Logger::Log(Logger::Level::Info, "Editor", "Application initialized.");
                PushLayer(layer);
            }

            void OnTermination() override {
                PopLayer(layer);
                Logger::Log(Logger::Level::Info, "Editor", "Application terminated.");
            }

            void OnUpdate(float deltaTime) override {
                if (Input::IsKeyHeld(KeyCode::Escape))
                    this->Terminate();
            }

            void OnEvent(Event& event) override {
                // Logger::Log(Logger::Level::Info, "Editor", ("Event received: " + event.ToString() + ".").c_str());
            }
    };

    Application* CreateApplication(int32_t argc, const uint8_t* argv[]) {
        ApplicationSpecification specification = {
            .name = ("Nickel2 v" + std::string(NK_VERSION_STRING)).c_str(), .workingDirectory = "./",
            .iconPath = "res/icon.png", .width = 1600, .height = 900, .decorated = true, .fullscreen = false,
            .maximized = false, .darkMode = true, .vSync = true, .resizable = false, .renderer = true
        };

        return new EditorApplication(specification);
    }
}