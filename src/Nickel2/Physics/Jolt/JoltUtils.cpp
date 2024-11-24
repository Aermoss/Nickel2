#include <Nickel2/nkpch.hpp>
#include <Nickel2/Physics/Jolt/JoltUtils.hpp>

namespace Nickel2::Utils {
    JPH::Vec3 ToJoltVector(const glm::vec3& vector) { return JPH::Vec3(vector.x, vector.y, vector.z); }
    JPH::Quat ToJoltQuat(const glm::quat& quat) { return JPH::Quat(quat.x, quat.y, quat.z, quat.w); }

    glm::vec3 FromJoltVector(const JPH::Vec3& vector) { return *(glm::vec3*) &vector; }
    glm::quat FromJoltQuat(const JPH::Quat& quat) { return *(glm::quat*) &quat; }

    JPH::EMotionQuality ToJoltMotionQuality(CollisionDetection mode) {
        switch (mode) {
            case CollisionDetection::Discrete: return JPH::EMotionQuality::Discrete;
            case CollisionDetection::Continuous: return JPH::EMotionQuality::LinearCast;
            default: return JPH::EMotionQuality::Discrete;
        }
    }

    CollisionDetection FromJoltMotionQuality(JPH::EMotionQuality mode) {
        switch (mode) {
            case JPH::EMotionQuality::Discrete: return CollisionDetection::Discrete;
            case JPH::EMotionQuality::LinearCast: return CollisionDetection::Continuous;
            default: return CollisionDetection::Discrete;
        }
    }

    JPH::EMotionType ToJoltMotionType(BodyType type) {
        switch (type) {
            case BodyType::Static: return JPH::EMotionType::Static;
            case BodyType::Dynamic: return JPH::EMotionType::Dynamic;
            case BodyType::Kinematic: return JPH::EMotionType::Kinematic;
            default: return JPH::EMotionType::Static;
        }
    }

    BodyType FromJoltMotionType(JPH::EMotionType type) {
        switch (type) {
            case JPH::EMotionType::Static: return BodyType::Static;
            case JPH::EMotionType::Dynamic: return BodyType::Dynamic;
            case JPH::EMotionType::Kinematic: return BodyType::Kinematic;
            default: return BodyType::Static;
        }
    }
}