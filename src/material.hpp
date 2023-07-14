#pragma once

#include "texture.hpp"

namespace nickel2 {
    struct Material {
        Texture *albedoMap, *roughnessMap, *metallicMap, *normalMap, *specularMap, *ambientMap;

        void destroy() {
            if (albedoMap != nullptr) {
                albedoMap->destroy();
                delete albedoMap;
            }
        }
    };
}