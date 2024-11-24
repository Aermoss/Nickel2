#include <Nickel2/nkpch.hpp>
#include <Nickel2/Scene/Scene.hpp>
#include <Nickel2/Scene/Entity.hpp>
#include <Nickel2/Physics/PhysicsSystem.hpp>
#include <Nickel2/Physics/PhysicsLayer.hpp>

namespace Nickel2 {
    Scene::Scene() : paused(false) {
        physicsScene = PhysicsSystem::CreateScene(this);
    }
    
    Scene::~Scene() {
        std::vector<uint64_t> queue;

        for (auto& [id, entity] : entities) {
            queue.push_back(id);
        } entities.clear();

        for (uint64_t id : queue) {
            if (IsEntityIDValid(id)) {
                Entity* entity = GetEntityByID(id);
                if (!entity->Destroyed()) entity->Destroy();
            }
        }
        
        lights.clear();
        physicsScene = nullptr;
    }

    void Scene::Update(float deltaTime) {
        if (this->paused) return;
        if (physicsScene != nullptr)
            physicsScene->Simulate(deltaTime);

        for (auto& [id, entity] : entities)
            entity->Update(deltaTime);
    }

    void Scene::InitEntity(Entity* entity, const std::string& name) {
        InitEntityWithID(entity, GetUniqueEntityID(), name);
    }

    void Scene::InitEntityWithID(Entity* entity, uint64_t id, const std::string& name) {
        entity->entityHandle = registry.create();
        entity->AddComponent<IDComponent>(id);
        entity->AddComponent<NameComponent>(name);
        entity->AddComponent<TransformComponent>();
        entity->AddComponent<RelationshipComponent>();
        entities[id] = entity;
    }

    Entity Scene::CreateEntity(const std::string& name) {
        return CreateEntityWithID(GetUniqueEntityID(), name);
    }

    Entity Scene::CreateEntityWithID(uint64_t id, const std::string& name) {
        Entity entity(registry.create(), this);
        entity.AddComponent<IDComponent>(id);
        entity.AddComponent<NameComponent>(name);
        entity.AddComponent<TransformComponent>();
        entity.AddComponent<RelationshipComponent>();
        entities[id] = &entity;
        return entity;
    }

    void Scene::DestroyEntity(Entity* entity) {
        entity->destroyState = true;
        entities.erase(entity->GetID());
        registry.destroy(entity->GetHandle());
    }

    Entity* Scene::GetEntityByID(uint64_t id) {
        NK_CORE_ASSERT(entities.find(id) != entities.end(), "Entity doesn't exist.");
        return entities[id];
    }

    Entity* Scene::GetEntityByHandle(entt::entity entityHandle) {
        for (auto& [id, entity] : entities) {
            if (entity->GetHandle() == entityHandle) return entity;
        } return nullptr;
    }

    bool Scene::CheckEntityID(Entity* entity, uint64_t id) {
        if (IsEntityIDValid(id)) {
            return entity == GetEntityByID(id);
        } else {
            return false;
        }
    }

    bool Scene::IsEntityValid(Entity* entity) {
        return !entity->destroyState;
    }

    bool Scene::IsEntityIDValid(uint64_t id) {
        return entities.find(id) != entities.end();
    }

    void Scene::SetPrimaryCamera(Camera* camera) {
        for (auto& entity : registry.view<CameraComponent>()) {
            registry.get<CameraComponent>(entity).camera->primary = false;
        } camera->primary = true;
    }

    void Scene::SetPrimaryCamera(CameraComponent& camera) {
        SetPrimaryCamera(camera.camera);
    }

    void Scene::SetPrimaryCamera(Entity* camera) {
        SetPrimaryCamera(camera->GetComponent<CameraComponent>().camera);
    }

    Camera* Scene::GetPrimaryCamera() {
        for (auto& entity : registry.view<CameraComponent>()) {
            CameraComponent& ref = registry.get<CameraComponent>(entity);
            if (ref.camera->primary) return ref.camera;
        } return nullptr;
    }

    CameraComponent* Scene::GetPrimaryCameraComponent() {
        for (auto& entity : registry.view<CameraComponent>()) {
            CameraComponent& ref = registry.get<CameraComponent>(entity);
            if (ref.camera->primary) return &ref;
        } return nullptr;
    }

    Entity* Scene::GetPrimaryCameraEntity() {
        for (auto& entity : registry.view<CameraComponent>()) {
            CameraComponent& ref = registry.get<CameraComponent>(entity);
            if (ref.camera->primary) return GetEntityByID(registry.get<IDComponent>(entity).id);
        } return nullptr;
    }

    std::vector<Light>& Scene::GetLights() {
        return lights;
    }

    void Scene::PushLight(Light light) {
        lights.push_back(light);
    }

    void Scene::PopLight() {
        lights.pop_back();
    }
}