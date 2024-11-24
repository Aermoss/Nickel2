#include <Nickel2/nkpch.hpp>
#include <Nickel2/Physics/PhysicsScene.hpp>
#include <Nickel2/Physics/PhysicsSystem.hpp>

namespace Nickel2 {
    PhysicsScene::PhysicsScene(Scene* scene) : scene(scene) {}

    PhysicsScene::~PhysicsScene() {
        scene = nullptr;
    }

    uint32_t PhysicsScene::CalculateSteps(float deltaTime) {
        PhysicsSettings& settings = PhysicsSystem::GetSettings();
        if (settings.fixedDelta <= 0.0f) return 1;
        
        uint32_t collisionSteps = 0;
        accumulator += deltaTime;

        while (accumulator >= settings.fixedDelta) {
            accumulator -= settings.fixedDelta;
            collisionSteps++;
        } return collisionSteps;
    }
}