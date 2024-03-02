#include <Nickel2/nkpch.hpp>
#include <Nickel2/Scene/Scene.hpp>
#include <Nickel2/Scene/Entity.hpp>

namespace Nickel2 {
    uint64_t Scene::getUniqueEntityID() {
        uint64_t id = 0;
        while (entities.find(id) != entities.end()) id++;
        return id;
    }

    Scene::Scene() : directionalLightPosition(glm::vec3(0.5f, 0.5f, 0.5f)), \
        paused(false), enableShadows(true), enableDirectionalLight(true) {}

    Scene::~Scene() {}

    bool Scene::isPaused() {
        return this->paused;
    }

    void Scene::setPaused(bool paused) {
        this->paused = paused;
    }

    void Scene::update(float deltaTime) {
        if (paused) return;

        for (auto& [id, entity] : entities)
            entity->update(deltaTime);
    }

    void Scene::initEntity(Entity* entity, const std::string& name) {
        initEntityWithID(entity, getUniqueEntityID(), name);
    }

    void Scene::initEntityWithID(Entity* entity, uint64_t id, const std::string& name) {
        entity->entityHandle = registry.create();
        entity->addComponent<IDComponent>(id);
        entity->addComponent<NameComponent>(name.empty() ? "Entity" : name);
        entity->addComponent<TransformComponent>();
        entities[id] = entity;
    }

    Entity Scene::createEntity(const std::string& name) {
        return createEntityWithID(getUniqueEntityID(), name);
    }

    Entity Scene::createEntityWithID(uint64_t id, const std::string& name) {
        Entity entity(registry.create(), this);
        entity.addComponent<IDComponent>(id);
        entity.addComponent<NameComponent>(name.empty() ? "Entity" : name);
        entity.addComponent<TransformComponent>();
        entities[id] = &entity;
        return entity;
    }

    void Scene::destroyEntity(Entity* entity) {
        entities.erase(entity->getID());
        registry.destroy(entity->getHandle());
    }

    Entity* Scene::getEntityByID(uint64_t id) {
        NICKEL2_ASSERT(entities.find(id) == entities.end(), "entity doesn't exist.");
        return entities[id];
    }

    std::vector<Light>& Scene::getLights() {
        return lights;
    }

    void Scene::setPrimaryCamera(Camera* camera) {
        for (auto& entity : registry.view<CameraComponent>()) {
            registry.get<CameraComponent>(entity).camera->primary = false;
        } camera->primary = true;
    }

    void Scene::setPrimaryCamera(CameraComponent& camera) {
        setPrimaryCamera(camera.camera);
    }

    void Scene::setPrimaryCamera(Entity* camera) {
        setPrimaryCamera(camera->getComponent<CameraComponent>().camera);
    }

    Camera* Scene::getPrimaryCamera() {
        for (auto& entity : registry.view<CameraComponent>()) {
            CameraComponent& ref = registry.get<CameraComponent>(entity);
            if (ref.camera->primary) return ref.camera;
        } return nullptr;
    }

    void Scene::pushLight(Light light) {
        lights.push_back(light);
    }

    void Scene::popLight() {
        lights.pop_back();
    }

    void Scene::destroy() {
        for (auto& [id, entity] : entities)
            entity->destroy();

        lights.clear();
    }
}