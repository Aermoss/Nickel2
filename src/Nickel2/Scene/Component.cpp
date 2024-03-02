#include <Nickel2/nkpch.hpp>
#include <Nickel2/Scene/Component.hpp>
#include <Nickel2/Scene/Entity.hpp>

namespace Nickel2 {
    IDComponent::IDComponent(Entity* entity, uint64_t id) : id(id) {}
    IDComponent::~IDComponent() {}
    
    NameComponent::NameComponent(Entity* entity, const std::string& name) : name(name) {}
    NameComponent::~NameComponent() {}

    TransformComponent::TransformComponent(Entity* entity) {
        transform = new Transform();
    }

    TransformComponent::~TransformComponent() {}

    void TransformComponent::destroy() {
        transform->destroy();
        delete transform;
    }

    MeshComponent::MeshComponent(Entity* entity, const std::string& path) {
        mesh = new Mesh(entity->getComponent<TransformComponent>().transform, path);
    }

    MeshComponent::~MeshComponent() {}

    void MeshComponent::destroy() {
        mesh->destroy();
        delete mesh;
    }

    CameraComponent::CameraComponent(Entity* entity, Window* window, float fov, float near, float far, uint32_t flags) {
        camera = new Camera(window, entity->getComponent<TransformComponent>().transform, fov, near, far, flags);
    }

    CameraComponent::~CameraComponent() {}

    void CameraComponent::destroy() {
        camera->destroy();
        delete camera;
    }

    ListenerComponent::ListenerComponent(Entity* entity, Camera* camera) {
        listener = new Listener(camera, entity->getComponent<TransformComponent>().transform);
    }

    ListenerComponent::ListenerComponent(Entity* entity, CameraComponent& camera) {
        listener = new Listener(camera.camera, entity->getComponent<TransformComponent>().transform);
    }

    ListenerComponent::~ListenerComponent() {}

    void ListenerComponent::destroy() {
        listener->destroy();
        delete listener;
    }

    SourceComponent::SourceComponent(Entity* entity, Listener* listener, const char* filePath, bool looping, float pitch, float gain) {
        source = new Source(listener, entity->getComponent<TransformComponent>().transform, filePath, looping, pitch, gain);
    }

    SourceComponent::SourceComponent(Entity* entity, ListenerComponent& listener, const char* filePath, bool looping, float pitch, float gain) {
        source = new Source(listener.listener, entity->getComponent<TransformComponent>().transform, filePath, looping, pitch, gain);
    }

    SourceComponent::~SourceComponent() {}

    void SourceComponent::destroy() {
        source->destroy();
        delete source;
    }
}