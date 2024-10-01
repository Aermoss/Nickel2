#pragma once

#include <glm/glm.hpp>

namespace Nickel2 {
    struct Vertex {
        glm::vec3 position;
        glm::vec2 texCoord;
        glm::vec3 normal;

        Vertex(glm::vec3 position) : position(position) {}
        Vertex(glm::vec3 position, glm::vec2 texCoord) : position(position), texCoord(texCoord) {}
        Vertex(glm::vec3 position, glm::vec2 texCoord, glm::vec3 normal) : position(position), texCoord(texCoord), normal(normal) {}
    };
}