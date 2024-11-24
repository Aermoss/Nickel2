#include <Nickel2/nkpch.hpp>
#include <Nickel2/Physics/PhysicsShapes.hpp>
#include <Nickel2/Physics/PhysicsSystem.hpp>
#include <Nickel2/Physics/PhysicsAPI.hpp>

#include <Nickel2/Physics/Jolt/JoltShapes.hpp>

namespace Nickel2 {
    std::shared_ptr<ImmutableCompoundShape> ImmutableCompoundShape::Create(Entity* entity) {
        switch (PhysicsAPI::GetAPI()) {
            case PhysicsAPIType::Jolt: return std::make_shared<JoltImmutableCompoundShape>(entity);
            default: return nullptr;
        }
    }

    std::shared_ptr<MutableCompoundShape> MutableCompoundShape::Create(Entity* entity) {
        switch (PhysicsAPI::GetAPI()) {
            case PhysicsAPIType::Jolt: return std::make_shared<JoltMutableCompoundShape>(entity);
            default: return nullptr;
        }
    }

    std::shared_ptr<BoxShape> BoxShape::Create(Entity* entity) {
        switch (PhysicsAPI::GetAPI()) {
            case PhysicsAPIType::Jolt: return std::make_shared<JoltBoxShape>(entity);
            default: return nullptr;
        }
    }

    std::shared_ptr<SphereShape> SphereShape::Create(Entity* entity) {
        switch (PhysicsAPI::GetAPI()) {
            case PhysicsAPIType::Jolt: return std::make_shared<JoltSphereShape>(entity);
            default: return nullptr;
        }
    }

    std::shared_ptr<CapsuleShape> CapsuleShape::Create(Entity* entity) {
        switch (PhysicsAPI::GetAPI()) {
            case PhysicsAPIType::Jolt: return std::make_shared<JoltCapsuleShape>(entity);
            default: return nullptr;
        }
    }

    std::shared_ptr<ConvexMeshShape> ConvexMeshShape::Create(Entity* entity) {
        return PhysicsSystem::GetMeshCookingFactory()->CookConvexMesh(entity);
    }

    std::shared_ptr<TriangleMeshShape> TriangleMeshShape::Create(Entity* entity) {
        return PhysicsSystem::GetMeshCookingFactory()->CookTriangleMesh(entity);
    }
}