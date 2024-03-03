#pragma once

#include "Scene.hpp"
#include "Component.hpp"

#include <entt/entt.hpp>

namespace Nickel2 {
    class Entity {
        private:
            entt::entity entityHandle;
            Scene* scene;

        public:
            Entity(entt::entity handle, Scene* scene);
            Entity(Scene* scene);
            ~Entity();

            virtual void onCreate() {}
            virtual void onUpdate(float deltaTime) {}
            virtual void onDestroy() {}

            template<typename T, typename... Args> T& addComponent(Args&&... args)  {
                NICKEL2_ASSERT(hasComponent<T>(), "entity already has that component.");
                T& component = scene->registry.emplace<T>(entityHandle, this, std::forward<Args>(args)...);
                return component;
            }

            template<typename T, typename... Args> T& addOrReplaceComponent(Args&&... args) {
                T& component = scene->registry.emplace_or_replace<T>(entityHandle, this, std::forward<Args>(args)...);
                return component;
            }

            template<typename T> T& getComponent() {
                NICKEL2_ASSERT(!hasComponent<T>(), "entity does not have that component.");
                return scene->registry.get<T>(entityHandle);
            }

            template<typename T> bool hasComponent() {
                return scene->registry.all_of<T>(entityHandle);
            }

            template<typename T> void removeComponent() {
                NICKEL2_ASSERT(!hasComponent<T>(), "entity does not have that component.");
                scene->registry.remove<T>(entityHandle);
            }

            operator bool() const { return entityHandle != entt::null; }
            operator entt::entity() const { return entityHandle; }
            operator uint32_t() const { return (uint32_t) entityHandle; }

            void update(float deltaTime);
            uint64_t getID();
            Scene* getScene();
            entt::entity getHandle();
            const std::string& getName();
            void destroy();

            friend bool operator==(const Entity& left, const Entity& right);
            friend bool operator!=(const Entity& left, const Entity& right);

            friend class Scene;
    };
}