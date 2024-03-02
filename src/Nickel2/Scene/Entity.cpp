#include <Nickel2/nkpch.hpp>
#include <Nickel2/Scene/Entity.hpp>

namespace Nickel2 {
    Entity::Entity(entt::entity handle, Nickel2::Scene* scene)
        : entityHandle(handle), scene(scene) {

    }

    Entity::Entity(Nickel2::Scene* scene) : scene(scene) {
        scene->initEntity(this);
        this->onCreate();
    }

    Entity::~Entity() {

    }

    void Entity::update(float deltaTime) {
        this->onUpdate(deltaTime);

        if (hasComponent<ListenerComponent>())
            getComponent<ListenerComponent>()->update();
    }

    uint64_t Entity::getID() {
        return getComponent<IDComponent>().id;
    }

    Scene* Entity::getScene() {
        return scene;
    }

    entt::entity Entity::getHandle() {
        return entityHandle;
    }

    const std::string& Entity::getName() {
        return getComponent<NameComponent>().name;
    }

    void Entity::destroy() {
        this->onDestroy();

        if (hasComponent<MeshComponent>())
            getComponent<MeshComponent>().destroy();

        if (hasComponent<TransformComponent>())
            getComponent<TransformComponent>().destroy();

        if (hasComponent<CameraComponent>())
            getComponent<CameraComponent>().destroy();

        if (hasComponent<ListenerComponent>())
            getComponent<ListenerComponent>().destroy();

        if (hasComponent<SourceComponent>())
            getComponent<SourceComponent>().destroy();
            
        scene->destroyEntity(this);
    }

    bool operator==(const Entity& left, const Entity& right) {
        return left.entityHandle == right.entityHandle && left.scene == right.scene;
    }

    bool operator!=(const Entity& left, const Entity& right) {
        return !(left == right);
    }
}