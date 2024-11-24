#pragma once

#include <Nickel2/Physics/PhysicsScene.hpp>
#include <Nickel2/Physics/Jolt/JoltLayerInterface.hpp>
#include <Nickel2/Physics/Jolt/JoltBody.hpp>

#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/BodyFilter.h>

namespace Nickel2 {
    class JoltScene : public PhysicsScene {
        private:
            std::unique_ptr<JPH::PhysicsSystem> physicsSystem;
            std::unique_ptr<JoltLayerInterface> layerInterface;
            std::unordered_map<uint64_t, std::shared_ptr<JoltBody>> rigidBodies;

        public:
            JoltScene(Scene* scene);
            ~JoltScene();

            void Simulate(float deltaTime) override;

            virtual std::shared_ptr<PhysicsBody> CreateBody(Entity* entity) override;
            virtual void DestroyBody(std::shared_ptr<PhysicsBody> body) override;

            static JPH::BodyInterface& GetBodyInterface(bool inShouldLock = true);
            static const JPH::BodyLockInterface& GetBodyLockInterface(bool inShouldLock = true);
            static JPH::PhysicsSystem& GetJoltSystem();
    };
}