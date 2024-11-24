#include <Nickel2/nkpch.hpp>
#include <Nickel2/Physics/Jolt/JoltShapes.hpp>
#include <Nickel2/Physics/Jolt/JoltBinaryStream.hpp>
#include <Nickel2/Physics/Jolt/JoltAPI.hpp>

#include <Nickel2/Physics/PhysicsSystem.hpp>

#include <Jolt/Physics/Collision/Shape/ConvexHullShape.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>

namespace Nickel2 {
    JoltImmutableCompoundShape::JoltImmutableCompoundShape(Entity* entity) : ImmutableCompoundShape(entity) {}

    void JoltImmutableCompoundShape::SetMaterial(const ColliderMaterial& material) {}

    void JoltImmutableCompoundShape::Register(const std::shared_ptr<PhysicsShape>& shape) {
        TransformComponent worldTransform(entity);
        worldTransform.DecomposeTransform(entity->GetComponent<TransformComponent>().GetWorldTransform());
        const TransformComponent& shapeTransform = entity->GetComponent<TransformComponent>();

        JPH::Shape* nativeShape = static_cast<JPH::Shape*>(shape->GetNativeShape());
        JPH::Vec3 translation = JPH::Vec3::sZero();
        JPH::Quat rotation = JPH::Quat::sIdentity();

        if (entity != shape->GetEntity()) {
            translation = Utils::ToJoltVector(shapeTransform.GetTranslation() * worldTransform.GetScale());
            rotation = Utils::ToJoltQuat(shapeTransform.GetRotation());
        }

        settings.AddShape(translation, rotation, nativeShape);
    }

    void JoltImmutableCompoundShape::Flush() {
        JoltAPI* api = static_cast<JoltAPI*>(PhysicsSystem::GetAPI());
        JPH::ShapeSettings::ShapeResult result = settings.Create(*api->GetTempAllocator());

        if (result.HasError()) {
            NK_CORE_ERROR_TAG("Physics", ("Failed to create immutable compound shape, error: " + result.GetError()).c_str());
            return;
        }

        shape = Utils::CastJoltRef<JPH::StaticCompoundShape>(result.Get());
        shape->SetUserData(reinterpret_cast<JPH::uint64>(this));
    }

    JoltMutableCompoundShape::JoltMutableCompoundShape(Entity* entity) : MutableCompoundShape(entity) {}

    void JoltMutableCompoundShape::SetMaterial(const ColliderMaterial& material) {}

    void JoltMutableCompoundShape::Register(const std::shared_ptr<PhysicsShape>& shape) {
        TransformComponent worldTransform(entity);
        worldTransform.DecomposeTransform(entity->GetComponent<TransformComponent>().GetWorldTransform());
        const TransformComponent& shapeTransform = entity->GetComponent<TransformComponent>();

        JPH::Shape* nativeShape = static_cast<JPH::Shape*>(shape->GetNativeShape());
        JPH::Vec3 translation = JPH::Vec3::sZero();
        JPH::Quat rotation = JPH::Quat::sIdentity();

        if (entity != shape->GetEntity()) {
            translation = Utils::ToJoltVector(shapeTransform.GetTranslation() * worldTransform.GetScale());
            rotation = Utils::ToJoltQuat(shapeTransform.GetRotation());
        }

        settings.AddShape(translation, rotation, nativeShape);
    }

    void JoltMutableCompoundShape::Flush() {
        JPH::ShapeSettings::ShapeResult result = settings.Create();

        if (result.HasError()) {
            NK_CORE_ERROR_TAG("Physics", ("Failed to create mutable compound shape, error: " + result.GetError()).c_str());
            return;
        }

        shape = Utils::CastJoltRef<JPH::MutableCompoundShape>(result.Get());
        shape->SetUserData(reinterpret_cast<JPH::uint64>(this));
    }

    JoltBoxShape::JoltBoxShape(Entity* entity) : BoxShape(entity) {
        TransformComponent worldTransform(entity);
        worldTransform.DecomposeTransform(entity->GetComponent<TransformComponent>().GetWorldTransform());
        BoxColliderComponent& colliderComponent = entity->GetComponent<BoxColliderComponent>();
        RigidBodyComponent& rigidBodyComponent = entity->GetComponent<RigidBodyComponent>();

        glm::vec3 halfColliderSize = glm::abs(worldTransform.GetScale() * colliderComponent.halfSize);
        float volume = halfColliderSize.x * 2.0f * halfColliderSize.y * 2.0f * halfColliderSize.z * 2.0f;

        material = JoltMaterial::FromColliderMaterial(colliderComponent.material);

        settings = new JPH::BoxShapeSettings(Utils::ToJoltVector(halfColliderSize), 0.01f);
        settings->mDensity = rigidBodyComponent.mass / volume;
        settings->mMaterial = material;

        JPH::RotatedTranslatedShapeSettings offsetShapeSettings(Utils::ToJoltVector(worldTransform.GetScale() * colliderComponent.offset), JPH::Quat::sIdentity(), settings);
        JPH::ShapeSettings::ShapeResult result = offsetShapeSettings.Create();

        if (result.HasError()) {
            NK_CORE_ERROR_TAG("Physics", ("Failed to create box shape, error: " + result.GetError()).c_str());
            return;
        }

        shape = Utils::CastJoltRef<JPH::RotatedTranslatedShape>(result.Get());
        shape->SetUserData(reinterpret_cast<JPH::uint64>(this));
    }
    
    void JoltBoxShape::SetMaterial(const ColliderMaterial& material) {
        this->material->SetFriction(material.friction);
        this->material->SetRestitution(material.restitution);
    }

    JoltSphereShape::JoltSphereShape(Entity* entity) : SphereShape(entity) {
        TransformComponent worldTransform(entity);
        worldTransform.DecomposeTransform(entity->GetComponent<TransformComponent>().GetWorldTransform());
        SphereColliderComponent& colliderComponent = entity->GetComponent<SphereColliderComponent>();
        RigidBodyComponent& rigidBodyComponent = entity->GetComponent<RigidBodyComponent>();

        float largestComponent = glm::abs(glm::max(worldTransform.GetScale().x, glm::max(worldTransform.GetScale().y, worldTransform.GetScale().z)));
        float scaledRadius = largestComponent * colliderComponent.radius;
        float volume = (4.0f / 3.0f) * glm::pi<float>() * (scaledRadius * scaledRadius * scaledRadius);

        material = JoltMaterial::FromColliderMaterial(colliderComponent.material);

        settings = new JPH::SphereShapeSettings(scaledRadius);
        settings->mDensity = rigidBodyComponent.mass / volume;
        settings->mMaterial = material;
        
        JPH::RotatedTranslatedShapeSettings offsetShapeSettings(Utils::ToJoltVector(worldTransform.GetScale() * colliderComponent.offset), JPH::Quat::sIdentity(), settings);
        JPH::ShapeSettings::ShapeResult result = offsetShapeSettings.Create();

        if (result.HasError()) {
            NK_CORE_ERROR_TAG("Physics", ("Failed to create sphere shape, error: " + result.GetError()).c_str());
            return;
        }

        shape = Utils::CastJoltRef<JPH::RotatedTranslatedShape>(result.Get());
        shape->SetUserData(reinterpret_cast<JPH::uint64>(this));
    }

    void JoltSphereShape::SetMaterial(const ColliderMaterial& material) {
        this->material->SetFriction(material.friction);
        this->material->SetRestitution(material.restitution);
    }

    JoltCapsuleShape::JoltCapsuleShape(Entity* entity) : CapsuleShape(entity) {
        TransformComponent worldTransform(entity);
        worldTransform.DecomposeTransform(entity->GetComponent<TransformComponent>().GetWorldTransform());
        CapsuleColliderComponent& colliderComponent = entity->GetComponent<CapsuleColliderComponent>();
        RigidBodyComponent& rigidBodyComponent = entity->GetComponent<RigidBodyComponent>();

        float largestRadius = glm::abs(glm::max(worldTransform.GetScale().x, worldTransform.GetScale().z));
        float scaledRadius = colliderComponent.radius * largestRadius;
        float scaledHalfHeight = glm::abs(colliderComponent.halfHeight * worldTransform.GetScale().y);
        float volume = glm::pi<float>() * (scaledRadius * scaledRadius) * ((4.0f / 3.0f) * scaledRadius + (scaledHalfHeight * 2.0f));

        material = JoltMaterial::FromColliderMaterial(colliderComponent.material);

        settings = new JPH::CapsuleShapeSettings(scaledHalfHeight, scaledRadius);
        settings->mDensity = rigidBodyComponent.mass / volume;
        settings->mMaterial = material;

        JPH::RotatedTranslatedShapeSettings offsetShapeSettings(Utils::ToJoltVector(worldTransform.GetScale() * colliderComponent.offset), JPH::Quat::sIdentity(), settings);
        JPH::ShapeSettings::ShapeResult result = offsetShapeSettings.Create();

        if (result.HasError()) {
            NK_CORE_ERROR_TAG("Physics", ("Failed to create capsule shape, error: " + result.GetError()).c_str());
            return;
        }

        shape = Utils::CastJoltRef<JPH::RotatedTranslatedShape>(result.Get());
        shape->SetUserData(reinterpret_cast<JPH::uint64>(this));
    }

    void JoltCapsuleShape::SetMaterial(const ColliderMaterial& material) {
        this->material->SetFriction(material.friction);
        this->material->SetRestitution(material.restitution);
    }

    JoltConvexMeshShape::JoltConvexMeshShape(Entity* entity, std::vector<MeshColliderData> colliderData) : ConvexMeshShape(entity) {
        TransformComponent worldTransform(entity);
        worldTransform.DecomposeTransform(entity->GetComponent<TransformComponent>().GetWorldTransform());
        MeshColliderComponent& colliderComponent = entity->GetComponent<MeshColliderComponent>();
        RigidBodyComponent& rigidBodyComponent = entity->GetComponent<RigidBodyComponent>();

        JoltBinaryStreamReader binaryReader(colliderData[0].collider);
        JPH::Shape::ShapeResult result = JPH::Shape::sRestoreFromBinaryState(binaryReader);

        if (result.HasError()) {
            NK_CORE_ERROR_TAG("Physics", ("Failed to create convex mesh shape, error: " + result.GetError()).c_str());
            return;
        }

        JPH::Ref<JPH::ConvexHullShape> convexShape = Utils::CastJoltRef<JPH::ConvexHullShape>(result.Get());
        convexShape->SetDensity(rigidBodyComponent.mass / convexShape->GetVolume());
        convexShape->SetMaterial(JoltMaterial::FromColliderMaterial(colliderComponent.material));

        shape = new JPH::ScaledShape(convexShape, Utils::ToJoltVector(worldTransform.GetScale()));
        shape->SetUserData(reinterpret_cast<JPH::uint64>(this));
    }

    void JoltConvexMeshShape::SetMaterial(const ColliderMaterial& material) {}

    JoltTriangleMeshShape::JoltTriangleMeshShape(Entity* entity, std::vector<MeshColliderData> colliderData) : TriangleMeshShape(entity) {
        TransformComponent worldTransform(entity);
        worldTransform.DecomposeTransform(entity->GetComponent<TransformComponent>().GetWorldTransform());
        JPH::StaticCompoundShapeSettings compoundShapeSettings;

        for (const auto& data : colliderData) {
            glm::quat submeshRotation;
            glm::vec3 submeshTranslation, submeshScale;
            Math::DecomposeTransform(data.transform, submeshTranslation, submeshRotation, submeshScale);

            JoltBinaryStreamReader binaryReader(data.collider);
            JPH::Shape::ShapeResult result = JPH::Shape::sRestoreFromBinaryState(binaryReader);

            if (result.HasError()) {
                NK_CORE_ERROR_TAG("Physics", "Failed to construct triangle mesh shape.");
                return;
            }

            compoundShapeSettings.AddShape(Utils::ToJoltVector(submeshTranslation), Utils::ToJoltQuat(submeshRotation), new JPH::ScaledShape(result.Get(), Utils::ToJoltVector(submeshScale * worldTransform.GetScale())));
        }

        JPH::Shape::ShapeResult result = compoundShapeSettings.Create();

        if (result.HasError()) {
            NK_CORE_ERROR_TAG("Physics", ("Failed to create triangle mesh shape, error: " + result.GetError()).c_str());
            return;
        }

        shape = Utils::CastJoltRef<JPH::StaticCompoundShape>(result.Get());
        shape->SetUserData(reinterpret_cast<JPH::uint64>(this));
    }

    void JoltTriangleMeshShape::SetMaterial(const ColliderMaterial& material) {}
}