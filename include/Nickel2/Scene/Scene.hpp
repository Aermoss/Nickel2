#pragma once

#include <cstdint>
#include <vector>

#include <entt/entt.hpp>

#include "../Core/Base.hpp"
#include "../Renderer/Light.hpp"

#include "Component.hpp"

namespace Nickel2 {
    class Entity;
    struct CameraComponent;

    class Scene {
        private:
            std::unordered_map<uint64_t, Entity*> entities;
            uint64_t getUniqueEntityID();

        public:
            entt::registry registry;
            std::vector<Light> lights;
            glm::vec3 directionalLightPosition;
            bool paused, enableShadows, enableDirectionalLight;

            Scene();
            ~Scene();

            template<typename... Components> auto getAllEntitiesWith() {
                return registry.view<Components...>();
            }

            bool isPaused();
            void setPaused(bool paused);
            void update(float deltaTime);
            void initEntity(Entity* entity, const std::string& name = std::string());
            void initEntityWithID(Entity* entity, uint64_t id, const std::string& name = std::string());
            Entity createEntity(const std::string& name = std::string());
            Entity createEntityWithID(uint64_t id, const std::string& name = std::string());
            void destroyEntity(Entity* entity);
            Entity* getEntityByID(uint64_t id);
            std::vector<Light>& getLights();
            void setPrimaryCamera(Camera* camera);
            void setPrimaryCamera(CameraComponent& camera);
            void setPrimaryCamera(Entity* camera);
            Camera* getPrimaryCamera();
            void pushLight(Light light);
            void popLight();
            void destroy();
    };
}