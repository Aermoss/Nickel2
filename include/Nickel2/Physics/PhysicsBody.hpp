#pragma once

#include <Nickel2/Scene/Entity.hpp>
#include <Nickel2/Physics/PhysicsShapes.hpp>

namespace Nickel2 {
    using ShapeArray = std::vector<std::shared_ptr<PhysicsShape>>;

    class PhysicsBody {
        protected:
            Entity* entity;
            std::unordered_map<ShapeType, ShapeArray> shapes;
            ActorAxis lockedAxes;

            void CreateCollisionShapesForEntity(Entity* entity, bool ignoreCompoundShapes = false);

        private:
            virtual void OnAxisLockUpdated(bool forceWake) = 0;

        public:
            PhysicsBody(Entity* entity) : entity(entity), lockedAxes(entity->GetComponent<RigidBodyComponent>().lockedAxes) {}
            virtual ~PhysicsBody() = default;

            uint32_t GetShapeCount(ShapeType type) { return shapes[type].size(); }
            const ShapeArray& GetShapes(ShapeType type) { return shapes[type]; }
            std::shared_ptr<PhysicsShape> GetShape(ShapeType type, uint32_t index) { return shapes[type][index]; }

            virtual void SetCollisionLayer(uint32_t layerId) = 0;

            virtual bool IsStatic() const = 0;
            virtual bool IsDynamic() const = 0;
            virtual bool IsKinematic() const = 0;

            virtual void MoveKinematic(const glm::vec3& targetPosition, const glm::quat& targetRotation, float deltaTime) = 0;
            virtual void Rotate(const glm::vec3& rotationTimesDelta) = 0;

            virtual void AddForce(const glm::vec3& force, ForceMode forceMode = ForceMode::Force, bool forceWake = true) = 0;
            virtual void AddForce(const glm::vec3& force, const glm::vec3& location, ForceMode forceMode = ForceMode::Force, bool forceWake = true) = 0;
            virtual void AddTorque(const glm::vec3& torque, bool forceWake = true) = 0;
            virtual void AddRadialImpulse(const glm::vec3& origin, float radius, float strength, FalloffMode falloff, bool velocityChange) = 0;

            virtual void SetGravityFactor(float factor) = 0;
            virtual float GetGravityFactor() const = 0;

            virtual void SetMass(float mass) = 0;
            virtual float GetMass() const = 0;

            virtual void SetLinearDrag(float drag) = 0;
            virtual float GetLinearDrag() const = 0;

            virtual void SetAngularDrag(float drag) = 0;
            virtual float GetAngularDrag() const = 0;

            virtual void SetLinearVelocity(const glm::vec3& velocity) = 0;
            virtual glm::vec3 GetLinearVelocity() const = 0;

            virtual void SetAngularVelocity(const glm::vec3& velocity) = 0;
            virtual glm::vec3 GetAngularVelocity() const = 0;

            virtual void SetMaxLinearVelocity(float velocity) = 0;
            virtual float GetMaxLinearVelocity() const = 0;

            virtual void SetMaxAngularVelocity(float velocity) = 0;
            virtual float GetMaxAngularVelocity() const = 0;

            virtual void SetTriggerState(bool state) = 0;
            virtual bool GetTriggerState() const = 0;

            virtual void SetSleepState(bool state) = 0;
            virtual bool GetSleepState() const = 0;

            virtual void SetCollisionDetectionMode(CollisionDetection mode) = 0;
            virtual CollisionDetection GetCollisionDetectionMode() const = 0;

            void SetAxisLock(ActorAxis axis, bool locked, bool forceWake);
            bool IsAxisLocked(ActorAxis axis) const;
            ActorAxis GetLockedAxes() const { return lockedAxes; }
            bool IsAllRotationLocked() const { return IsAxisLocked(ActorAxis::RotationX) && IsAxisLocked(ActorAxis::RotationY) && IsAxisLocked(ActorAxis::RotationZ); }

            Entity* GetEntity() { return entity; }
    };
}