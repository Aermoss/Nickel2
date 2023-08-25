#pragma once

#include "texture.hpp"

namespace nickel2 {
    class Texture;
    
    struct Material {
        std::string name;
        bool rotateTexture;
        int32_t textureScale;
        int32_t shadingMode;
        glm::vec3 albedo;
        Texture *albedoMap, *roughnessMap, *metallicMap, *normalMap, *specularMap, *ambientMap;
        float ambient, roughness, metallic, transparent;

        std::vector <Texture*> getTextures() {
            return {albedoMap, roughnessMap, metallicMap, normalMap, specularMap, ambientMap};
        }
    };
}