#include <Nickel2/nkpch.hpp>
#include <Nickel2/Scene/Entity.hpp>

namespace Nickel2 {
    Entity::Entity(entt::entity handle, Nickel2::Scene* scene)
        : entityHandle(handle), scene(scene) {
        this->OnCreate();
    }

    Entity::Entity(Nickel2::Scene* scene, const std::string& name) : scene(scene) {
        scene->InitEntity(this, name);
        this->OnCreate();
    }

    Entity::Entity(Nickel2::Scene* scene) : scene(scene) {
        scene->InitEntity(this);
        this->OnCreate();
    }

    void Entity::Update(float deltaTime) {
        this->OnUpdate(deltaTime);

        if (HasComponent<ListenerComponent>())
            GetComponent<ListenerComponent>()->Update(deltaTime);
    }

    void Entity::Destroy() {
        NK_CORE_ASSERT(!destroyState, "Entity is already destroyed.");
        this->OnDestroy();

        if (HasComponent<MeshComponent>())
            GetComponent<MeshComponent>().Destroy();

        if (HasComponent<SubmeshComponent>())
            GetComponent<SubmeshComponent>().Destroy();

        if (HasComponent<CameraComponent>())
            GetComponent<CameraComponent>().Destroy();

        if (HasComponent<ListenerComponent>())
            GetComponent<ListenerComponent>().Destroy();

        if (HasComponent<SourceComponent>())
            GetComponent<SourceComponent>().Destroy();

        scene->DestroyEntity(this);
    }

    bool operator==(const Entity& left, const Entity& right) {
        return left.entityHandle == right.entityHandle && left.scene == right.scene;
    }

    bool operator!=(const Entity& left, const Entity& right) {
        return !(left == right);
    }
}