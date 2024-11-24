#pragma once

#include <Nickel2/Physics/PhysicsTypes.hpp>

#include <Jolt/Math/Vec3.h>
#include <Jolt/Math/Quat.h>
#include <Jolt/Core/Reference.h>
#include <Jolt/Physics/Body/MotionQuality.h>
#include <Jolt/Physics/Body/MotionType.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Nickel2::Utils {
    JPH::Vec3 ToJoltVector(const glm::vec3& vector);
    JPH::Quat ToJoltQuat(const glm::quat& quat);

    glm::vec3 FromJoltVector(const JPH::Vec3& vector);
    glm::quat FromJoltQuat(const JPH::Quat& quat);

    template<typename T, typename U> JPH::Ref<T> CastJoltRef(JPH::RefConst<U> ref) {
        return JPH::Ref<T>(static_cast<T*>(const_cast<U*>(ref.GetPtr())));
    }

    template<typename T, typename U> JPH::Ref<T> CastJoltRef(const JPH::Ref<U> ref) {
        return JPH::Ref<T>(static_cast<T*>(const_cast<U*>(ref.GetPtr())));
    }

    JPH::EMotionQuality ToJoltMotionQuality(CollisionDetection mode);
    CollisionDetection FromJoltMotionQuality(JPH::EMotionQuality mode);

    JPH::EMotionType ToJoltMotionType(BodyType type);
    BodyType FromJoltMotionType(JPH::EMotionType type);
}