#pragma once

#include <glm/glm.hpp>

namespace Nickel2 {
    class Texture;
    
    struct Material {
        std::string name;
        int32_t shadingMode;
        glm::vec4 albedo;
        std::shared_ptr<Texture> albedoMap, roughnessMap, metallicMap, normalMap, specularMap, ambientMap;
        float ambient, roughness, metallic, transparent;
        int32_t textureScale;
        bool rotateTexture;

        std::vector<std::shared_ptr<Texture>> GetTextures() {
            return { albedoMap, roughnessMap, metallicMap, normalMap, specularMap, ambientMap };
        }
    };
}