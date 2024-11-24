#include <Nickel2/nkpch.hpp>
#include <Nickel2/Physics/Jolt/JoltScene.hpp>
#include <Nickel2/Physics/Jolt/JoltUtils.hpp>
#include <Nickel2/Physics/Jolt/JoltBody.hpp>
#include <Nickel2/Physics/Jolt/JoltAPI.hpp>

#include <Nickel2/Physics/PhysicsSystem.hpp>

#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/RayCast.h>
#include <Jolt/Physics/Collision/ShapeCast.h>
#include <Jolt/Physics/Collision/CollisionCollectorImpl.h>
#include <Jolt/Physics/Collision/CastResult.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Body/BodyLockMulti.h>
#include <Jolt/Core/Profiler.h>

namespace Nickel2 {
    static JoltScene* currentInstance = nullptr;

    class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter {
        public:
            bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override {
                const auto& layerData1 = PhysicsLayerManager::GetLayer(inLayer1);
                const auto& layerData2 = PhysicsLayerManager::GetLayer((uint8_t) inLayer2);

                if (layerData1.layerId == layerData2.layerId)
                    return layerData1.collidesWithSelf;

                return layerData1.collidesWith & layerData2.bitValue;
            }
    };

    class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter {
        public:
            bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override {
                const auto& layerData1 = PhysicsLayerManager::GetLayer(inObject1);
                const auto& layerData2 = PhysicsLayerManager::GetLayer(inObject2);

                if (layerData1.layerId == layerData2.layerId)
                    return layerData1.collidesWithSelf;

                return layerData1.collidesWith & layerData2.bitValue;
            }
    };

    static ObjectLayerPairFilterImpl objectVsObjectLayerFilter;
    static ObjectVsBroadPhaseLayerFilterImpl objectVsBroadPhaseLayerFilter;

    JoltScene::JoltScene(Scene* scene) : PhysicsScene(scene) {
        currentInstance = this;
        const auto& settings = PhysicsSystem::GetSettings();

        physicsSystem = std::make_unique<JPH::PhysicsSystem>();
        layerInterface = std::make_unique<JoltLayerInterface>();

        physicsSystem->Init(20240, 0, 65536, 20240, *layerInterface, objectVsBroadPhaseLayerFilter, objectVsObjectLayerFilter);
        physicsSystem->SetGravity(Utils::ToJoltVector(settings.gravity));        

        physicsSystem->SetPhysicsSettings({
            .mNumVelocitySteps = settings.velocitySolverIterations,
            .mNumPositionSteps = settings.positionSolverIterations
        });
    }

    JoltScene::~JoltScene() {
        rigidBodies.clear();
        layerInterface = nullptr, physicsSystem = nullptr, currentInstance = nullptr;
    }

    void JoltScene::Simulate(float deltaTime) {
        uint32_t collisionSteps = CalculateSteps(deltaTime);
        if (collisionSteps == 0) return;

        PhysicsSettings& settings = PhysicsSystem::GetSettings();
        const auto& bodyLockInterface = physicsSystem->GetBodyLockInterface();

        {
            JPH::BodyIDVector bodies;

            for (auto& [id, body] : rigidBodies)
                bodies.push_back(body->GetBodyID());

            JPH::BodyLockMultiWrite bodyLock(bodyLockInterface, bodies.data(), static_cast<int32_t>(bodies.size()));

            for (uint64_t i = 0; i < bodies.size(); i++) {
                JPH::Body* body = bodyLock.GetBody(i);
                if (!body || body->IsKinematic()) continue;

                Entity* entity = scene->GetEntityByID(static_cast<uint64_t>(body->GetUserData()));
                if (!entity) continue;

                auto& transformComponent = entity->GetComponent<TransformComponent>();
                JoltScene::GetBodyInterface(false).SetPosition(body->GetID(), Utils::ToJoltVector(transformComponent.GetTranslation()), JPH::EActivation::Activate);
                JoltScene::GetBodyInterface(false).SetRotation(body->GetID(), Utils::ToJoltQuat(transformComponent.GetRotation()), JPH::EActivation::Activate);
            }
        }

        JoltAPI* api = static_cast<JoltAPI*>(PhysicsSystem::GetAPI());
        physicsSystem->Update(settings.fixedDelta != 0.0f ? settings.fixedDelta : deltaTime, collisionSteps, api->GetTempAllocator(), api->GetJobThreadPool());

        {
            JPH::BodyIDVector activeBodies;
            physicsSystem->GetActiveBodies(JPH::EBodyType::RigidBody, activeBodies);
            JPH::BodyLockMultiWrite bodyLock(bodyLockInterface, activeBodies.data(), static_cast<int32_t>(activeBodies.size()));

            for (uint64_t i = 0; i < activeBodies.size(); i++) {
                JPH::Body* body = bodyLock.GetBody(i);
                if (!body || body->IsKinematic()) continue;

                Entity* entity = scene->GetEntityByID(static_cast<uint64_t>(body->GetUserData()));
                if (!entity) continue;

                auto& transformComponent = entity->GetComponent<TransformComponent>();
                transformComponent.SetTranslation(Utils::FromJoltVector(body->GetPosition()));

                if (!rigidBodies.at(entity->GetID())->IsAllRotationLocked())
                    transformComponent.SetRotation(Utils::FromJoltQuat(body->GetRotation()));
            }
        }
    }

    std::shared_ptr<PhysicsBody> JoltScene::CreateBody(Entity* entity) {
        auto it = rigidBodies.find(entity->GetID());
        if (it != rigidBodies.end()) return it->second;

        JPH::BodyInterface& bodyInterface = physicsSystem->GetBodyInterface();
        std::shared_ptr<JoltBody> rigidBody = std::make_shared<JoltBody>(entity, bodyInterface);

        if (rigidBody->GetBodyID().IsInvalid())
            return nullptr;

        bodyInterface.AddBody(rigidBody->GetBodyID(), JPH::EActivation::Activate);
        rigidBodies[entity->GetID()] = rigidBody;
        return rigidBody;
    }

    void JoltScene::DestroyBody(std::shared_ptr<PhysicsBody> body) {
        for (const auto& [key, value] : rigidBodies)
            if (value == body) rigidBodies.erase(key);
    }

    JPH::BodyInterface& JoltScene::GetBodyInterface(bool inShouldLock) {
        NK_CORE_ASSERT(currentInstance != nullptr);
        return inShouldLock ? currentInstance->physicsSystem->GetBodyInterface() : currentInstance->physicsSystem->GetBodyInterfaceNoLock();
    }

    const JPH::BodyLockInterface& JoltScene::GetBodyLockInterface(bool inShouldLock) {
        NK_CORE_ASSERT(currentInstance != nullptr);
        if (inShouldLock) return currentInstance->physicsSystem->GetBodyLockInterface();
        else return currentInstance->physicsSystem->GetBodyLockInterfaceNoLock();
    }

    JPH::PhysicsSystem& JoltScene::GetJoltSystem() {
        return *currentInstance->physicsSystem.get();
    }
}