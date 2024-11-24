#include <Nickel2/nkpch.hpp>
#include <Nickel2/Physics/Jolt/JoltBody.hpp>
#include <Nickel2/Physics/Jolt/JoltScene.hpp>
#include <Nickel2/Physics/Jolt/JoltUtils.hpp>

#include <Nickel2/Physics/PhysicsBody.hpp>

#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/StaticCompoundShape.h>
#include <Jolt/Physics/Collision/Shape/MutableCompoundShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>

#include <magic_enum.hpp>

using namespace magic_enum::bitwise_operators;

namespace Nickel2 {
    JoltBody::JoltBody(Entity* entity, JPH::BodyInterface& bodyInterface) : PhysicsBody(entity) {
        const auto& rigidBodyComponent = entity->GetComponent<RigidBodyComponent>();

        switch (rigidBodyComponent.bodyType) {
            case BodyType::Static: {
                CreateStaticBody(bodyInterface);
                break;
            } case BodyType::Dynamic: {

            } case BodyType::Kinematic: {
                CreateDynamicBody(bodyInterface);
                break;
            }
        }

        oldMotionType = Utils::ToJoltMotionType(rigidBodyComponent.bodyType);
    }

    JoltBody::~JoltBody() {
        JPH::BodyLockWrite lock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(lock.Succeeded());
        auto& body = lock.GetBody();

        if (axisLockConstraint != nullptr) {
            JoltScene::GetJoltSystem().RemoveConstraint(axisLockConstraint);
            axisLockConstraint = nullptr;
        }

        if (body.IsInBroadPhase()) JoltScene::GetBodyInterface(false).RemoveBody(this->id);
        else JoltScene::GetBodyInterface(false).DeactivateBody(this->id);

        shapes.clear();
        JoltScene::GetBodyInterface(false).DestroyBody(this->id);
    }

    void JoltBody::SetCollisionLayer(uint32_t layerId) {
        JoltScene::GetBodyInterface().SetObjectLayer(this->id, JPH::ObjectLayer(layerId));
    }

    bool JoltBody::IsStatic() const {
        return JoltScene::GetBodyInterface().GetMotionType(this->id) == JPH::EMotionType::Static;
    }

    bool JoltBody::IsDynamic() const {
        return JoltScene::GetBodyInterface().GetMotionType(this->id) == JPH::EMotionType::Dynamic;
    }

    bool JoltBody::IsKinematic() const {
        return JoltScene::GetBodyInterface().GetMotionType(this->id) == JPH::EMotionType::Kinematic;
    }

    void JoltBody::MoveKinematic(const glm::vec3& targetPosition, const glm::quat& targetRotation, float deltaTime) {
        JPH::BodyLockWrite bodyLock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(bodyLock.Succeeded());
        JPH::Body& body = bodyLock.GetBody();

        if (body.GetMotionType() != JPH::EMotionType::Kinematic) {
            NK_CORE_ERROR_TAG("Physics", "Can't move a non-kinematic rigid body kinematically.");
            return;
        }

        body.MoveKinematic(Utils::ToJoltVector(targetPosition), Utils::ToJoltQuat(targetRotation), deltaTime);
    }


    void JoltBody::Rotate(const glm::vec3& rotationTimesDelta) {
        JPH::BodyLockWrite bodyLock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(bodyLock.Succeeded());
        JPH::Body& body = bodyLock.GetBody();

        if (body.GetMotionType() != JPH::EMotionType::Kinematic) {
            NK_CORE_ERROR_TAG("Physics", "Can't rotate a non-kinematic rigid body kinematically.");
            return;
        }

        body.AddRotationStep(Utils::ToJoltVector(rotationTimesDelta));
    }

    void JoltBody::AddForce(const glm::vec3& force, ForceMode forceMode, bool forceWake) {
        if (!IsDynamic()) {
            NK_CORE_ERROR_TAG("Physics", "Can't apply force to a non-dynamic rigid body.");
            return;
        }

        JPH::BodyLockWrite lock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(lock.Succeeded());
        auto& body = lock.GetBody();

        switch (forceMode) {
            case ForceMode::Force: {
                body.AddForce(Utils::ToJoltVector(force));
                break;
            } case ForceMode::Impulse: {
                body.AddImpulse(Utils::ToJoltVector(force));
                break;
            } case ForceMode::VelocityChange: {
                body.SetLinearVelocityClamped(body.GetLinearVelocity() + Utils::ToJoltVector(force));
                if (body.GetLinearVelocity().IsNearZero()) return;
                break;
            } case ForceMode::Acceleration: {
                body.AddForce(Utils::ToJoltVector(force) / body.GetMotionProperties()->GetInverseMass());
                break;
            }
        }

        if (body.IsInBroadPhase() && !body.IsActive())
            JoltScene::GetBodyInterface(false).ActivateBody(this->id);
    }


    void JoltBody::AddForce(const glm::vec3& force, const glm::vec3& location, ForceMode forceMode, bool forceWake) {
        if (!IsDynamic()) {
            NK_CORE_ERROR_TAG("Physics", "Can't apply force to a non-dynamic rigid body.");
            return;
        }

        JPH::BodyLockWrite lock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(lock.Succeeded());
        auto& body = lock.GetBody();

        switch (forceMode) {
            case ForceMode::Force: {
                body.AddForce(Utils::ToJoltVector(force), Utils::ToJoltVector(location));
                break;
            } case ForceMode::Impulse: {
                body.AddImpulse(Utils::ToJoltVector(force), Utils::ToJoltVector(location));
                break;
            } case ForceMode::VelocityChange: {
                body.SetLinearVelocityClamped(body.GetLinearVelocity() + Utils::ToJoltVector(force));
                if (body.GetLinearVelocity().IsNearZero()) return;
                break;
            } case ForceMode::Acceleration: {
                body.AddForce(Utils::ToJoltVector(force) / body.GetMotionProperties()->GetInverseMass(), Utils::ToJoltVector(location));
                break;
            }
        }

        if (body.IsInBroadPhase() && !body.IsActive())
            JoltScene::GetBodyInterface(false).ActivateBody(this->id);
    }


    void JoltBody::AddTorque(const glm::vec3& torque, bool forceWake) {
        if (!IsDynamic()) {
            NK_CORE_ERROR_TAG("Physics", "Can't apply torque to a non-dynamic rigid body.");
            return;
        }

        JoltScene::GetBodyInterface().AddTorque(this->id, Utils::ToJoltVector(torque));
    }


    void JoltBody::AddRadialImpulse(const glm::vec3& origin, float radius, float strength, FalloffMode falloff, bool velocityChange) {
        if (!IsDynamic()) {
            NK_CORE_ERROR_TAG("Physics", "Can't apply radial impulse to a non-dynamic rigid body.");
            return;
        }

        JPH::BodyLockRead readLock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(readLock.Succeeded());
        const JPH::Body& bodyRead = readLock.GetBody();
        glm::vec3 centerOfMassPosition = Utils::FromJoltVector(bodyRead.GetCenterOfMassPosition());
        readLock.ReleaseLock();

        glm::vec3 direction = centerOfMassPosition - origin;
        float distance = glm::length(direction);

        if (distance > radius)
            return;

        direction = glm::normalize(direction);
        float impulseMagnitude = strength;

        if (falloff == FalloffMode::Linear)
            impulseMagnitude *= (1.0f - (distance / radius));

        glm::vec3 impulse = direction * impulseMagnitude;
        JPH::BodyLockWrite writeLock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(writeLock.Succeeded());
        JPH::Body& bodyWrite = writeLock.GetBody();

        if (velocityChange) {
            JPH::Vec3 linearVelocity = bodyWrite.GetLinearVelocity() + Utils::ToJoltVector(impulse);
            bodyWrite.SetLinearVelocityClamped(linearVelocity);
            if (linearVelocity.IsNearZero()) return;
        } else {
            bodyWrite.AddImpulse(Utils::ToJoltVector(impulse));
        }

        if (!bodyWrite.IsActive())
            JoltScene::GetBodyInterface(false).ActivateBody(this->id);
    }

    void JoltBody::SetGravityFactor(float factor) {
        if (!IsDynamic()) {
            NK_CORE_ERROR_TAG("Physics", "Can't set gravity factor of a non-dynamic rigid body.");
            return;
        }

        JPH::BodyLockWrite lock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(lock.Succeeded());
        lock.GetBody().GetMotionProperties()->SetGravityFactor(factor);
    }

    float JoltBody::GetGravityFactor() const {
        if (!IsDynamic()) {
            NK_CORE_ERROR_TAG("Physics", "Can't get gravity factor of a non-dynamic rigid body.");
            return 0.0f;
        }

        JPH::BodyLockWrite lock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(lock.Succeeded());
        return lock.GetBody().GetMotionProperties()->GetGravityFactor();
    }

    void JoltBody::SetMass(float mass) {
        if (IsStatic()) {
            NK_CORE_ERROR_TAG("Physics", "Can't set mass of a static rigid body.");
            return;
        }

        JPH::BodyLockWrite lock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(lock.Succeeded());
        JPH::Body& body = lock.GetBody();
        JPH::MassProperties massProperties = body.GetShape()->GetMassProperties();
        massProperties.ScaleToMass(mass);
        massProperties.mInertia(3, 3) = 1.0f;
        body.GetMotionProperties()->SetMassProperties(JPH::EAllowedDOFs::All, massProperties);
    }

    float JoltBody::GetMass() const {
        if (IsStatic()) {
            NK_CORE_ERROR_TAG("Physics", "Can't get mass of a static rigid body.");
            return 0.0f;
        }

        JPH::BodyLockRead lock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(lock.Succeeded());
        return 1.0f / lock.GetBody().GetMotionProperties()->GetInverseMass();
    }

    void JoltBody::SetLinearDrag(float drag) {
        if (IsStatic()) {
            NK_CORE_ERROR_TAG("Physics", "Can't set linear drag of a static rigid body.");
            return;
        }

        JPH::BodyLockWrite lock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(lock.Succeeded());
        lock.GetBody().GetMotionProperties()->SetLinearDamping(drag);
    }

    float JoltBody::GetLinearDrag() const {
        if (IsStatic()) {
            NK_CORE_ERROR_TAG("Physics", "Can't set linear drag of a static rigid body.");
            return 0.0f;
        }

        JPH::BodyLockWrite lock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(lock.Succeeded());
        return lock.GetBody().GetMotionProperties()->GetLinearDamping();
    }

    void JoltBody::SetAngularDrag(float drag) {
        if (IsStatic()) {
            NK_CORE_ERROR_TAG("Physics", "Can't set angular drag of a static rigid body.");
            return;
        }

        JPH::BodyLockWrite lock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(lock.Succeeded());
        lock.GetBody().GetMotionProperties()->SetAngularDamping(drag);
    }

    float JoltBody::GetAngularDrag() const {
        if (IsStatic()) {
            NK_CORE_ERROR_TAG("Physics", "Can't set angular drag of a static rigid body.");
            return 0.0f;
        }

        JPH::BodyLockWrite lock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(lock.Succeeded());
        return lock.GetBody().GetMotionProperties()->GetAngularDamping();
    }

    void JoltBody::SetLinearVelocity(const glm::vec3& velocity) {
        if (!IsDynamic()) {
            NK_CORE_ERROR_TAG("Physics", "Can't set linear velocity of a non-dynamic rigid body.");
            return;
        }

        JoltScene::GetBodyInterface().SetLinearVelocity(this->id, Utils::ToJoltVector(velocity));
    }

    glm::vec3 JoltBody::GetLinearVelocity() const {
        if (IsStatic()) return glm::vec3(0.0f);
        return Utils::FromJoltVector(JoltScene::GetBodyInterface().GetLinearVelocity(this->id));
    }

    void JoltBody::SetAngularVelocity(const glm::vec3& velocity) {
        if (!IsDynamic()) {
            NK_CORE_ERROR_TAG("Physics", "Can't set angular velocity of a non-dynamic rigid body.");
            return;
        }

        JPH::BodyLockWrite lock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(lock.Succeeded());
        JPH::Body& body = lock.GetBody();
        JPH::Vec3 value = Utils::ToJoltVector(velocity);
        body.SetAngularVelocityClamped(value);

        if (!body.IsActive() && !value.IsNearZero() && body.IsInBroadPhase())
            JoltScene::GetBodyInterface(false).ActivateBody(this->id);
    }

    glm::vec3 JoltBody::GetAngularVelocity() const {
        if (IsStatic()) return glm::vec3(0.0f);
        return Utils::FromJoltVector(JoltScene::GetBodyInterface().GetAngularVelocity(this->id));
    }

    void JoltBody::SetMaxLinearVelocity(float velocity) {
        if (!IsDynamic()) {
            NK_CORE_ERROR_TAG("Physics", "Can't set max linear velocity of a non-dynamic rigid body.");
            return;
        }

        JPH::BodyLockWrite lock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(lock.Succeeded());
        lock.GetBody().GetMotionProperties()->SetMaxLinearVelocity(velocity);
    }

    float JoltBody::GetMaxLinearVelocity() const {
        if (!IsDynamic()) {
            NK_CORE_ERROR_TAG("Physics", "Can't get max linear velocity of a non-dynamic rigid body.");
            return 0.0f;
        }

        JPH::BodyLockRead lock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(lock.Succeeded());
        return lock.GetBody().GetMotionProperties()->GetMaxLinearVelocity();
    }

    void JoltBody::SetMaxAngularVelocity(float velocity) {
        if (!IsDynamic()) {
            NK_CORE_ERROR_TAG("Physics", "Can't set max angular velocity of a non-dynamic rigid body.");
            return;
        }

        JPH::BodyLockWrite lock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(lock.Succeeded());
        lock.GetBody().GetMotionProperties()->SetMaxAngularVelocity(velocity);
    }

    float JoltBody::GetMaxAngularVelocity() const {
        if (!IsDynamic()) {
            NK_CORE_ERROR_TAG("Physics", "Can't get max angular velocity of a non-dynamic rigid body.");
            return 0.0f;
        }

        JPH::BodyLockRead lock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(lock.Succeeded());
        return lock.GetBody().GetMotionProperties()->GetMaxAngularVelocity();
    }

    void JoltBody::SetTriggerState(bool state) {
        JPH::BodyLockWrite lock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(lock.Succeeded());
        lock.GetBody().SetIsSensor(state);
    }

    bool JoltBody::GetTriggerState() const {
        JPH::BodyLockRead lock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(lock.Succeeded());
        return lock.GetBody().IsSensor();
    }

    void JoltBody::SetSleepState(bool state) {
        if (IsStatic()) {
            NK_CORE_ERROR_TAG("Physics", "Can't change sleep state of a static rigid body.");
            return;
        }

        JPH::BodyLockWrite lock(JoltScene::GetBodyLockInterface(), this->id);
        if (!lock.Succeeded()) return;

        if (state) {
            JoltScene::GetBodyInterface(false).DeactivateBody(this->id);
        } else if (lock.GetBody().IsInBroadPhase()) {
            JoltScene::GetBodyInterface(false).ActivateBody(this->id);
        }
    }

    bool JoltBody::GetSleepState() const {
        if (IsStatic()) return false;
        JPH::BodyLockRead lock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(lock.Succeeded());
        return !lock.GetBody().IsActive();
    }

    void JoltBody::SetCollisionDetectionMode(CollisionDetection mode) {
        JoltScene::GetBodyInterface().SetMotionQuality(this->id, Utils::ToJoltMotionQuality(mode));
    }

    CollisionDetection JoltBody::GetCollisionDetectionMode() const {
        return Utils::FromJoltMotionQuality(JoltScene::GetBodyInterface().GetMotionQuality(this->id));
    }

    void JoltBody::CreateStaticBody(JPH::BodyInterface& bodyInterface) {
        Scene* scene = entity->GetScene();
        NK_CORE_ASSERT(scene, "Scene is null.");

        TransformComponent worldTransform(entity);
        worldTransform.DecomposeTransform(entity->GetComponent<TransformComponent>().GetWorldTransform());
        const auto& rigidBodyComponent = entity->GetComponent<RigidBodyComponent>();

        CreateCollisionShapesForEntity(entity);

        if (shapes.empty())
            return;

        JPH::Shape* firstShape = nullptr;

        if (shapes.find(ShapeType::ImmutableCompound) != shapes.end()) {
            firstShape = static_cast<JPH::Shape*>(shapes.at(ShapeType::ImmutableCompound)[0]->GetNativeShape());
        } else if (shapes.find(ShapeType::MutableCompound) != shapes.end()) {
            firstShape = static_cast<JPH::Shape*>(shapes.at(ShapeType::MutableCompound)[0]->GetNativeShape());
        } else {
            for (const auto& [shapeType, shapeStorage] : shapes) {
                firstShape = static_cast<JPH::Shape*>(shapeStorage[0]->GetNativeShape());
                break;
            }
        }

        if (firstShape == nullptr) {
            NK_CORE_ERROR_TAG("Physics", "Failed to create rigid body, no collision shape.");
            shapes.clear();
            return;
        }

        JPH::BodyCreationSettings bodySettings(
            firstShape,
            Utils::ToJoltVector(worldTransform.GetTranslation()),
            Utils::ToJoltQuat(glm::normalize(worldTransform.GetRotation())),
            JPH::EMotionType::Static,
            JPH::ObjectLayer(rigidBodyComponent.layerId)
        );

        bodySettings.mAllowSleeping = false;
        bodySettings.mIsSensor = rigidBodyComponent.isTrigger;
        bodySettings.mAllowDynamicOrKinematic = rigidBodyComponent.enableDynamicTypeChange;
        bodySettings.mOverrideMassProperties = JPH::EOverrideMassProperties::CalculateInertia;
        bodySettings.mMassPropertiesOverride.mMass = rigidBodyComponent.mass;

        JPH::Body* body = bodyInterface.CreateBody(bodySettings);

        if (body == nullptr) {
            NK_CORE_ERROR_TAG("Physics", "Failed to create rigid body, not enough space.");
            return;
        }

        body->SetUserData(entity->GetID());
        this->id = body->GetID();
    }

    void JoltBody::CreateDynamicBody(JPH::BodyInterface& bodyInterface) {
        Scene* scene = entity->GetScene();
        NK_CORE_ASSERT(scene, "Scene is null.");

        TransformComponent worldTransform(entity);
        worldTransform.DecomposeTransform(entity->GetComponent<TransformComponent>().GetWorldTransform());
        auto& rigidBodyComponent = entity->GetComponent<RigidBodyComponent>();

        CreateCollisionShapesForEntity(entity);
        if (shapes.empty()) return;
        JPH::Shape* firstShape = nullptr;

        if (shapes.find(ShapeType::ImmutableCompound) != shapes.end()) {
            firstShape = static_cast<JPH::Shape*>(shapes.at(ShapeType::ImmutableCompound)[0]->GetNativeShape());
        } else if (shapes.find(ShapeType::MutableCompound) != shapes.end()) {
            firstShape = static_cast<JPH::Shape*>(shapes.at(ShapeType::MutableCompound)[0]->GetNativeShape());
        } else {
            for (const auto& [shapeType, shapeStorage] : shapes) {
                firstShape = static_cast<JPH::Shape*>(shapeStorage[0]->GetNativeShape());
                break;
            }
        }

        if (firstShape == nullptr) {
            NK_CORE_ERROR_TAG("Physics", "Failed to create rigid body, no collision shape.");
            shapes.clear();
            return;
        }

        if (!PhysicsLayerManager::IsLayerValid(rigidBodyComponent.layerId))
            rigidBodyComponent.layerId = 0;

        JPH::BodyCreationSettings bodySettings(
            firstShape,
            Utils::ToJoltVector(worldTransform.GetTranslation()),
            Utils::ToJoltQuat(glm::normalize(worldTransform.GetRotation())),
            Utils::ToJoltMotionType(rigidBodyComponent.bodyType),
            JPH::ObjectLayer(rigidBodyComponent.layerId)
        );

        bodySettings.mAllowSleeping = true;
        bodySettings.mLinearDamping = rigidBodyComponent.linearDrag;
        bodySettings.mAngularDamping = rigidBodyComponent.angularDrag;
        bodySettings.mIsSensor = rigidBodyComponent.isTrigger;
        bodySettings.mGravityFactor = rigidBodyComponent.disableGravity ? 0.0f : 1.0f;
        bodySettings.mLinearVelocity = Utils::ToJoltVector(rigidBodyComponent.initialLinearVelocity);
        bodySettings.mAngularVelocity = Utils::ToJoltVector(rigidBodyComponent.initialAngularVelocity);
        bodySettings.mMaxLinearVelocity = rigidBodyComponent.maxLinearVelocity;
        bodySettings.mMaxAngularVelocity = rigidBodyComponent.maxAngularVelocity;
        bodySettings.mMotionQuality = Utils::ToJoltMotionQuality(rigidBodyComponent.collisionDetection);
        bodySettings.mOverrideMassProperties = JPH::EOverrideMassProperties::CalculateInertia;
        bodySettings.mMassPropertiesOverride.mMass = rigidBodyComponent.mass;

        JPH::Body* body = bodyInterface.CreateBody(bodySettings);

        if (body == nullptr) {
            NK_CORE_ERROR_TAG("Physics", "Failed to create rigid body, not enough space.");
            return;
        }

        body->SetUserData(entity->GetID());
        this->id = body->GetID();

        if (rigidBodyComponent.lockedAxes != ActorAxis::None)
            CreateAxisLockConstraint(*body);
    }

    void JoltBody::CreateAxisLockConstraint(JPH::Body& body) {
        JPH::SixDOFConstraintSettings constraintSettings;
        constraintSettings.mPosition1 = constraintSettings.mPosition2 = body.GetCenterOfMassPosition();

        if ((lockedAxes & ActorAxis::TranslationX) != ActorAxis::None)
            constraintSettings.MakeFixedAxis(JPH::SixDOFConstraintSettings::TranslationX);

        if ((lockedAxes & ActorAxis::TranslationY) != ActorAxis::None)
            constraintSettings.MakeFixedAxis(JPH::SixDOFConstraintSettings::TranslationY);

        if ((lockedAxes & ActorAxis::TranslationZ) != ActorAxis::None)
            constraintSettings.MakeFixedAxis(JPH::SixDOFConstraintSettings::TranslationZ);

        if ((lockedAxes & ActorAxis::RotationX) != ActorAxis::None)
            constraintSettings.MakeFixedAxis(JPH::SixDOFConstraintSettings::RotationX);

        if ((lockedAxes & ActorAxis::RotationY) != ActorAxis::None)
            constraintSettings.MakeFixedAxis(JPH::SixDOFConstraintSettings::RotationY);

        if ((lockedAxes & ActorAxis::RotationZ) != ActorAxis::None)
            constraintSettings.MakeFixedAxis(JPH::SixDOFConstraintSettings::RotationZ);

        axisLockConstraint = (JPH::SixDOFConstraint*) constraintSettings.Create(JPH::Body::sFixedToWorld, body);
        JoltScene::GetJoltSystem().AddConstraint(axisLockConstraint);
    }

    void JoltBody::OnAxisLockUpdated(bool forceWake) {
        JPH::BodyLockWrite bodyLock(JoltScene::GetBodyLockInterface(), this->id);
        NK_CORE_ASSERT(bodyLock.Succeeded());

        if (axisLockConstraint) {
            JoltScene::GetJoltSystem().RemoveConstraint(axisLockConstraint);
            axisLockConstraint = nullptr;
        }

        if (lockedAxes != ActorAxis::None)
            CreateAxisLockConstraint(bodyLock.GetBody());
    }
}