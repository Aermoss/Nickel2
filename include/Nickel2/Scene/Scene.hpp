#pragma once

#include <cstdint>
#include <vector>

#include <entt/entt.hpp>

#include "../Core/Base.hpp"
#include "../Renderer/Light.hpp"
#include "../Renderer/Camera.hpp"

#include "Component.hpp"

namespace Nickel2 {
    class Entity;
    class Camera;
    
    struct CameraComponent;

    class Scene {
        private:
            std::unordered_map<uint64_t, Entity*> entities;
            std::vector<Light> lights;
            entt::registry registry;
            bool paused = false;

            uint64_t GetUniqueEntityID() {
                uint64_t id = 1;
                while (entities.find(id) != entities.end()) id++;
                return id;
            }

        public:
            Scene();
            ~Scene();

            template<typename... Components> std::vector<Entity*> GetAllEntitiesWith() {
                std::vector<Entity*> entities;

                for (auto& entity : registry.view<Components...>()) {
                    entities.push_back(GetEntityByHandle(entity));
                } return entities;
            }

            template<typename... Components> uint64_t GetEntityCountWith() {
                uint64_t count;

                for (auto& entity : registry.view<Components...>()) {
                    if (GetEntityByHandle(entity)->Destroyed()) count++;
                } return count;
            }

            bool Paused() { return this->paused; }
            bool Resumed() { return !this->paused; }

            void Pause() { this->paused = true; }
            void Resume() { this->paused = false; }

            void Update(float deltaTime);

            void InitEntity(Entity* entity, const std::string& name = std::string());
            void InitEntityWithID(Entity* entity, uint64_t id, const std::string& name = std::string());

            Entity CreateEntity(const std::string& name = std::string());
            Entity CreateEntityWithID(uint64_t id, const std::string& name = std::string());

            void DestroyEntity(Entity* entity);

            Entity* GetEntityByID(uint64_t id);
            Entity* GetEntityByHandle(entt::entity entityHandle);

            bool CheckEntityID(Entity* entity, uint64_t id);

            bool IsEntityValid(Entity* entity);
            bool IsEntityIDValid(uint64_t id);

            void SetPrimaryCamera(Camera* camera);
            void SetPrimaryCamera(CameraComponent& camera);
            void SetPrimaryCamera(Entity* camera);

            Camera* GetPrimaryCamera();
            CameraComponent* GetPrimaryCameraComponent();
            Entity* GetPrimaryCameraEntity();

            std::vector<Light>& GetLights();
            void PushLight(Light light);
            void PopLight();

            void Destroy();

            friend class Entity;
    };
}