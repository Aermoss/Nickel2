#pragma once

#include "Texture.hpp"

namespace Nickel2 {
    class Texture;
    
    struct Material {
        std::string name;
        bool rotateTexture;
        int32_t textureScale;
        int32_t shadingMode;
        glm::vec4 albedo;
        Texture *albedoMap, *roughnessMap, *metallicMap, *normalMap, *specularMap, *ambientMap;
        float ambient, roughness, metallic, transparent;

        std::vector <Texture*> getTextures() {
            return {albedoMap, roughnessMap, metallicMap, normalMap, specularMap, ambientMap};
        }
    };
}