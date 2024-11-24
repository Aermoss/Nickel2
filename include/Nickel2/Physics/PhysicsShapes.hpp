#pragma once

#include "ColliderMaterial.hpp"

namespace Nickel2 {
    class Entity;
    
    enum class ShapeType {
        Box, Sphere, Capsule, ConvexMesh, TriangleMesh, ImmutableCompound, MutableCompound
    };

    namespace ShapeUtils {
        inline const char* ShapeTypeToString(ShapeType type) {
            switch (type) {
                case ShapeType::ImmutableCompound: return "Immutable Compound";
                case ShapeType::MutableCompound: return "Mutable Compound";
                case ShapeType::Box: return "Box";
                case ShapeType::Sphere: return "Sphere";
                case ShapeType::Capsule: return "Capsule";
                case ShapeType::ConvexMesh: return "Convex Mesh";
                case ShapeType::TriangleMesh: return "Triangle Mesh";
                default: return "Unknown";
            }
        }
    }

    class PhysicsShape {
        protected:
            Entity* entity;
            ShapeType type;

        public:
            PhysicsShape(Entity* entity, ShapeType type) : entity(entity), type(type) {}
            virtual ~PhysicsShape() = default;

            virtual void SetMaterial(const ColliderMaterial& material) = 0;

            virtual uint32_t GetNumShapes() const = 0;
            virtual void* GetNativeShape() const = 0;

            ShapeType GetType() const { return type; }
            Entity* GetEntity() const { return entity; }
    };

    class CompoundShape : public PhysicsShape {
        public:
            CompoundShape(Entity* entity, bool isImmutable) : PhysicsShape(entity, isImmutable ? ShapeType::ImmutableCompound : ShapeType::MutableCompound) {}
            virtual ~CompoundShape() = default;

            virtual void Register(const std::shared_ptr<PhysicsShape>& shape) = 0;
            virtual void Flush() = 0;
    };

    class ImmutableCompoundShape : public CompoundShape {
        public:
            ImmutableCompoundShape(Entity* entity) : CompoundShape(entity, true) {}
            virtual ~ImmutableCompoundShape() = default;

            static std::shared_ptr<ImmutableCompoundShape> Create(Entity* entity);
    };

    class MutableCompoundShape : public CompoundShape {
        public:
            MutableCompoundShape(Entity* entity) : CompoundShape(entity, false) {}
            virtual ~MutableCompoundShape() = default;

            static std::shared_ptr<MutableCompoundShape> Create(Entity* entity);
    };

    class BoxShape : public PhysicsShape {
        public:
            BoxShape(Entity* entity) : PhysicsShape(entity, ShapeType::Box) {}
            virtual ~BoxShape() = default;

            virtual glm::vec3 GetHalfSize() const = 0;

            static std::shared_ptr<BoxShape> Create(Entity* entity);
    };

    class SphereShape : public PhysicsShape {
        public:
            SphereShape(Entity* entity) : PhysicsShape(entity, ShapeType::Sphere) {}
            virtual ~SphereShape() = default;

            virtual float GetRadius() const = 0;

            static std::shared_ptr<SphereShape> Create(Entity* entity);
    };

    class CapsuleShape : public PhysicsShape {
        public:
            CapsuleShape(Entity* entity) : PhysicsShape(entity, ShapeType::Capsule) {}
            virtual ~CapsuleShape() = default;

            virtual float GetRadius() const = 0;
            virtual float GetHeight() const = 0;

            static std::shared_ptr<CapsuleShape> Create(Entity* entity);
    };

    class ConvexMeshShape : public PhysicsShape {
        public:
            ConvexMeshShape(Entity* entity) : PhysicsShape(entity, ShapeType::ConvexMesh) {}
            virtual ~ConvexMeshShape() = default;

            static std::shared_ptr<ConvexMeshShape> Create(Entity* entity);
    };

    class TriangleMeshShape : public PhysicsShape {
        public:
            TriangleMeshShape(Entity* entity) : PhysicsShape(entity, ShapeType::TriangleMesh) {}
            virtual ~TriangleMeshShape() = default;

            static std::shared_ptr<TriangleMeshShape> Create(Entity* entity);
    };
}