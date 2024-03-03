#pragma once

#include <glm/glm.hpp>

namespace Nickel2 {
    struct Light {
        glm::vec3 position;
        glm::vec3 color;
        float brightness;

        Light(glm::vec3 position) : position(position) {}
        Light(glm::vec3 position, glm::vec3 color) : position(position), color(color) {}
        Light(glm::vec3 position, glm::vec3 color, float brightness) : position(position), color(color), brightness(brightness) {}
    };
}