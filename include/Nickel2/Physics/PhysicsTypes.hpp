#pragma once

#include <Nickel2/Core/Base.hpp>

namespace Nickel2 {
    enum class CookingResult : uint32_t {
        None = 0, Success, Failure
    };

    enum class ForceMode : uint32_t {
        Force = 0, Impulse, VelocityChange, Acceleration
    };

    enum class ActorAxis : uint32_t {
        None = 0, RotationX = BIT(3), RotationY = BIT(4), RotationZ = BIT(5), Rotation = RotationX | RotationY | RotationZ,
        TranslationX = BIT(0), TranslationY = BIT(1), TranslationZ = BIT(2), Translation = TranslationX | TranslationY | TranslationZ
    };

    enum class CollisionComplexity : uint32_t {
        Simple = 0, Complex
    };

    enum class CollisionDetection : uint32_t {
        Discrete = 0, Continuous
    };

    enum class CollisionFlags : uint32_t {
        None = 0, Sides = BIT(0), Above = BIT(1), Below = BIT(2)
    };

    enum class BodyType : uint32_t {
        Static = 0, Dynamic, Kinematic
    };

    enum class FalloffMode : uint32_t {
        Constant = 0, Linear
    };
}