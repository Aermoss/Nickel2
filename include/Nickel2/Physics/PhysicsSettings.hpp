#pragma once

#include "PhysicsTypes.hpp"

#include <glm/glm.hpp>

namespace Nickel2 {
    struct PhysicsSettings {
        glm::vec3 gravity = { 0.0f, -9.81f, 0.0f };
        uint32_t positionSolverIterations = 2;
        uint32_t velocitySolverIterations = 10;
        float fixedDelta = 0.0f;
    };
}