#include <Nickel2/nkpch.hpp>
#include <Nickel2/Physics/PhysicsBody.hpp>
#include <Nickel2/Physics/PhysicsSystem.hpp>

#include <magic_enum.hpp>

using namespace magic_enum::bitwise_operators;

namespace Nickel2 {
    void PhysicsBody::SetAxisLock(ActorAxis axis, bool locked, bool forceWake) {
        if (locked) { lockedAxes |= axis; }
        else { lockedAxes &= ~axis; }
        OnAxisLockUpdated(forceWake);
    }

    bool PhysicsBody::IsAxisLocked(ActorAxis axis) const {
        return (lockedAxes & axis) != ActorAxis::None;
    }

    void PhysicsBody::CreateCollisionShapesForEntity(Entity* entity, bool ignoreCompoundShapes) {
        Scene* scene = entity->GetScene();

        if (entity->HasComponent<CompoundColliderComponent>() && !ignoreCompoundShapes) {
            CompoundColliderComponent& colliderComponent = entity->GetComponent<CompoundColliderComponent>();
            std::shared_ptr<CompoundShape> compoundShape = nullptr;

            if (colliderComponent.isImmutable) { compoundShape = MutableCompoundShape::Create(entity); }
            else { compoundShape = MutableCompoundShape::Create(entity); }

            for (uint64_t id : colliderComponent.compoundedColliderEntities) {
                Entity* colliderEntity = scene->GetEntityByID(id);
                if (!colliderEntity) continue;
                CreateCollisionShapesForEntity(colliderEntity, colliderEntity == this->entity || colliderEntity == entity);
            }

            for (const auto& [shapeType, shapeStorage] : shapes)
                for (const auto& shape : shapeStorage)
                    compoundShape->Register(shape);

            compoundShape->Flush();
            shapes[compoundShape->GetType()].push_back(compoundShape);
            return;
        }

        if (entity->HasComponent<BoxColliderComponent>())
            shapes[ShapeType::Box].push_back(BoxShape::Create(entity));

        if (entity->HasComponent<SphereColliderComponent>())
            shapes[ShapeType::Sphere].push_back(SphereShape::Create(entity));

        if (entity->HasComponent<CapsuleColliderComponent>())
            shapes[ShapeType::Capsule].push_back(CapsuleShape::Create(entity));

        if (entity->HasComponent<MeshColliderComponent>()) {
            MeshColliderComponent& colliderComponent = entity->GetComponent<MeshColliderComponent>();

            if (!entity->HasComponent<MeshComponent>()) {
                NK_CORE_ERROR_TAG("Physics", "Entity doesn't have a mesh component.");
                return;
            }

            switch (colliderComponent.complexity) {
                case CollisionComplexity::Simple: {
                    shapes[ShapeType::ConvexMesh].push_back(ConvexMeshShape::Create(entity));
                    break;
                } case CollisionComplexity::Complex: {
                    shapes[ShapeType::TriangleMesh].push_back(TriangleMeshShape::Create(entity));
                    break;
                } default: {
                    break;
                }
            }
        }
    }
}