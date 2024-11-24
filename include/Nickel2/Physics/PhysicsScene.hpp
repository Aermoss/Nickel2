#pragma once

#include <Nickel2/Physics/PhysicsSettings.hpp>
#include <Nickel2/Physics/PhysicsBody.hpp>

namespace Nickel2 {
    class PhysicsScene {
        protected:
            Scene* scene;
            float accumulator = 0.0f;

            uint32_t CalculateSteps(float deltaTime);

        public:
            PhysicsScene(Scene* scene);
            virtual ~PhysicsScene();

            virtual void Simulate(float deltaTime) = 0;

            virtual std::shared_ptr<PhysicsBody> CreateBody(Entity* entity) = 0;
            virtual void DestroyBody(std::shared_ptr<PhysicsBody> body) = 0;
    };
}