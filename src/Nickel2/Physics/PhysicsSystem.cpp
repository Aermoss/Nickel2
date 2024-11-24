#include <Nickel2/nkpch.hpp>
#include <Nickel2/Physics/PhysicsSystem.hpp>

namespace Nickel2 {
    std::unique_ptr<PhysicsAPI> PhysicsSystem::api = PhysicsAPI::Create();

	std::shared_ptr<PhysicsScene> PhysicsSystem::CreateScene(Scene* scene) {
        return api->CreateScene(scene);
    }
}