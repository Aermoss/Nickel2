#pragma once

#include "../Core/Assert.hpp"

#include "Scene.hpp"
#include "Component.hpp"

#include <entt/entt.hpp>

namespace Nickel2 {
    class Entity {
        private:
            bool destroyState = false;
            entt::entity entityHandle;
            Scene* scene;

        public:
            Entity(entt::entity handle, Scene* scene);
            Entity(Scene* scene, const std::string& name);
            Entity(Scene* scene);

            virtual ~Entity() = default;

            virtual void OnCreate() {}
            virtual void OnUpdate(float deltaTime) {}
            virtual void OnDestroy() {}

            template<typename T, typename... Args> T& AddComponent(Args&&... args)  {
                NK_CORE_ASSERT(!HasComponent<T>(), "Entity already has that component.");
                T& component = scene->registry.emplace<T>(entityHandle, this, std::forward<Args>(args)...);
                return component;
            }

            template<typename T, typename... Args> T& AddOrReplaceComponent(Args&&... args) {
                T& component = scene->registry.emplace_or_replace<T>(entityHandle, this, std::forward<Args>(args)...);
                return component;
            }

            template<typename T> T& GetComponent() {
                NK_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have that component.");
                return scene->registry.get<T>(entityHandle);
            }

            template<typename T> bool HasComponent() {
                return scene->registry.all_of<T>(entityHandle);
            }

            template<typename T> void RemoveComponent() {
                NK_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have that component.");
                scene->registry.remove<T>(entityHandle);
            }

            operator bool() const { return entityHandle != entt::null; }
            operator entt::entity() const { return entityHandle; }
            operator uint32_t() const { return (uint32_t) entityHandle; }

            Scene* GetScene() const { return scene; }
            entt::entity GetHandle() const { return entityHandle; }
            
            uint64_t GetID() { return GetComponent<IDComponent>().id; }
            const std::string& GetName() { return GetComponent<NameComponent>().name; }

            void Update(float deltaTime);
            void Destroy();

            bool Destroyed() const { return destroyState; }

            friend bool operator==(const Entity& left, const Entity& right);
            friend bool operator!=(const Entity& left, const Entity& right);

            friend class Scene;
    };
}