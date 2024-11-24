#pragma once

#include <Nickel2/Physics/PhysicsBody.hpp>

#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/Constraints/SixDOFConstraint.h>

namespace Nickel2 {
    class JoltBody : public PhysicsBody {
        private:
            JPH::BodyID id;
            JPH::EMotionType oldMotionType = JPH::EMotionType::Static;
            JPH::SixDOFConstraint* axisLockConstraint = nullptr;

            void CreateStaticBody(JPH::BodyInterface& bodyInterface);
            void CreateDynamicBody(JPH::BodyInterface& bodyInterface);

            void CreateAxisLockConstraint(JPH::Body& body);
            virtual void OnAxisLockUpdated(bool forceWake) override;

        public:
            JoltBody(Entity* entity, JPH::BodyInterface& bodyInterface);
            ~JoltBody();

            virtual void SetCollisionLayer(uint32_t layerId) override;

            virtual bool IsStatic() const override;
            virtual bool IsDynamic() const override;
            virtual bool IsKinematic() const override;

            virtual void MoveKinematic(const glm::vec3& targetPosition, const glm::quat& targetRotation, float deltaTime) override;
            virtual void Rotate(const glm::vec3& rotationTimesDelta) override;
            
            virtual void AddForce(const glm::vec3& force, ForceMode forceMode = ForceMode::Force, bool forceWake = true) override;
            virtual void AddForce(const glm::vec3& force, const glm::vec3& location, ForceMode forceMode = ForceMode::Force, bool forceWake = true) override;
            virtual void AddTorque(const glm::vec3& torque, bool forceWake = true) override;
            virtual void AddRadialImpulse(const glm::vec3& origin, float radius, float strength, FalloffMode falloff, bool velocityChange) override;

            virtual void SetGravityFactor(float factor) override;
            virtual float GetGravityFactor() const override;

            virtual void SetMass(float mass) override;
            virtual float GetMass() const override;

            virtual void SetLinearDrag(float drag) override;
            virtual float GetLinearDrag() const override;

            virtual void SetAngularDrag(float drag) override;
            virtual float GetAngularDrag() const override;

            virtual void SetLinearVelocity(const glm::vec3& velocity) override;
            virtual glm::vec3 GetLinearVelocity() const override;

            virtual void SetAngularVelocity(const glm::vec3& velocity) override;
            virtual glm::vec3 GetAngularVelocity() const override;

            virtual void SetMaxLinearVelocity(float velocity) override;
            virtual float GetMaxLinearVelocity() const override;

            virtual void SetMaxAngularVelocity(float velocity) override;
            virtual float GetMaxAngularVelocity() const override;

            virtual void SetTriggerState(bool state) override;
            virtual bool GetTriggerState() const override;

            virtual void SetSleepState(bool state) override;
            virtual bool GetSleepState() const override;

            virtual void SetCollisionDetectionMode(CollisionDetection mode) override;
            virtual CollisionDetection GetCollisionDetectionMode() const override;

            JPH::BodyID GetBodyID() const { return this->id; }

	        friend class JoltScene;
    };
}