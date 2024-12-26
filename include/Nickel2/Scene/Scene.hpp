#pragma once

#include <cstdint>
#include <vector>

#include <entt/entt.hpp>

#include "../Core/Base.hpp"
#include "../Renderer/Camera.hpp"

#include "Component.hpp"

namespace Nickel2 {
    class Entity;
    class PhysicsScene;
    class Camera;

    struct CameraComponent;

    class Scene {
        private:
            std::shared_ptr<PhysicsScene> physicsScene;
            std::unordered_map<uint64_t, Entity*> entities;
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

            template<typename Component> std::vector<Component*> GetInstancesOf() {
                std::vector<Component*> components = {};

                for (auto& entity : registry.view<Component>()) {
                    components.push_back(&registry.get<Component>(entity));
                } return components;
            }

            template<typename... Components> std::vector<Entity*> GetEntitiesWith() {
                std::vector<Entity*> entities = {};

                for (auto& entity : registry.view<Components...>()) {
                    entities.push_back(GetEntityByHandle(entity));
                } return entities;
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

            std::shared_ptr<PhysicsScene> GetPhysicsScene() {
                return physicsScene;
            }

            friend class Entity;
    };
}