#pragma once

#include <Nickel2/Physics/PhysicsShapes.hpp>
#include <Nickel2/Physics/Jolt/JoltMaterial.hpp>
#include <Nickel2/Physics/Jolt/JoltUtils.hpp>

#include <Jolt/Physics/Collision/Shape/StaticCompoundShape.h>
#include <Jolt/Physics/Collision/Shape/MutableCompoundShape.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>

namespace Nickel2 {
    class JoltImmutableCompoundShape : public ImmutableCompoundShape {
        private:
            JPH::Ref<JPH::StaticCompoundShape> shape = nullptr;
            JPH::StaticCompoundShapeSettings settings;

        public:
            JoltImmutableCompoundShape(Entity* entity);
            ~JoltImmutableCompoundShape() {}

            virtual void SetMaterial(const ColliderMaterial& material) override;

            virtual uint32_t GetNumShapes() const override { return 1; }
            virtual void* GetNativeShape() const override { return shape.GetPtr(); };

            virtual void Register(const std::shared_ptr<PhysicsShape>& shape) override;
            virtual void Flush() override;
    };

    class JoltMutableCompoundShape : public MutableCompoundShape {
        private:
            JPH::Ref<JPH::MutableCompoundShape> shape = nullptr;
            JPH::MutableCompoundShapeSettings settings;

        public:
            JoltMutableCompoundShape(Entity* entity);
            ~JoltMutableCompoundShape() {}

            virtual void SetMaterial(const ColliderMaterial& material) override;

            virtual uint32_t GetNumShapes() const override { return 1; }
            virtual void* GetNativeShape() const override { return shape.GetPtr(); };

            virtual void Register(const std::shared_ptr<PhysicsShape>& shape) override;
            virtual void Flush() override;
    };

    class JoltBoxShape : public BoxShape {
        private:
            JoltMaterial* material = nullptr;
            JPH::Ref<JPH::RotatedTranslatedShape> shape = nullptr;
            JPH::Ref<JPH::BoxShapeSettings> settings;

        public:
            JoltBoxShape(Entity* entity);
            ~JoltBoxShape() {}

            virtual void SetMaterial(const ColliderMaterial& material) override;

            virtual uint32_t GetNumShapes() const override { return 1; }
            virtual void* GetNativeShape() const override { return shape.GetPtr(); }

            virtual glm::vec3 GetHalfSize() const override { return Utils::FromJoltVector(((const JPH::BoxShape*) shape->GetInnerShape())->GetHalfExtent()); }
    };

    class JoltSphereShape : public SphereShape {
        private:
            JoltMaterial* material = nullptr;
            JPH::Ref<JPH::RotatedTranslatedShape> shape = nullptr;
            JPH::Ref<JPH::SphereShapeSettings> settings;

        public:
            JoltSphereShape(Entity* entity);
            ~JoltSphereShape() {}

            virtual void SetMaterial(const ColliderMaterial& material) override;

            virtual uint32_t GetNumShapes() const override { return 1; }
            virtual void* GetNativeShape() const override { return shape.GetPtr(); }

            virtual float GetRadius() const override { return ((const JPH::CapsuleShape*) shape->GetInnerShape())->GetRadius(); }
    };

    class JoltCapsuleShape : public CapsuleShape {
        private:
            JoltMaterial* material = nullptr;
            JPH::Ref<JPH::RotatedTranslatedShape> shape = nullptr;
            JPH::Ref<JPH::CapsuleShapeSettings> settings;

        public:
            JoltCapsuleShape(Entity* entity);
            ~JoltCapsuleShape() {}

            virtual void SetMaterial(const ColliderMaterial& material) override;

            virtual uint32_t GetNumShapes() const override { return 1; }
            virtual void* GetNativeShape() const override { return shape.GetPtr(); }

            virtual float GetRadius() const override { return ((const JPH::CapsuleShape*) shape->GetInnerShape())->GetRadius(); }
            virtual float GetHeight() const override { return ((const JPH::CapsuleShape*) shape->GetInnerShape())->GetHalfHeightOfCylinder() * 2.0f; }
    };

    class JoltConvexMeshShape : public ConvexMeshShape {
        private:
            JPH::Ref<JPH::ScaledShape> shape;

        public:
            JoltConvexMeshShape(Entity* entity, std::vector<MeshColliderData> colliderData);
            ~JoltConvexMeshShape() {}

            virtual void SetMaterial(const ColliderMaterial& material) override;

            virtual uint32_t GetNumShapes() const override { return 1; }
            virtual void* GetNativeShape() const override { return shape.GetPtr(); }
    };

    class JoltTriangleMeshShape : public TriangleMeshShape {
        private:
            JPH::Ref<JPH::StaticCompoundShape> shape;

        public:
            JoltTriangleMeshShape(Entity* entity, std::vector<MeshColliderData> colliderData);
            ~JoltTriangleMeshShape() {}

            virtual void SetMaterial(const ColliderMaterial& material) override;

            virtual uint32_t GetNumShapes() const override { return shape->GetNumSubShapes(); }
            virtual void* GetNativeShape() const override { return shape.GetPtr(); }
    };
}