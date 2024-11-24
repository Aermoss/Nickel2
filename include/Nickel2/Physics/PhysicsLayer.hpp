#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace Nickel2 {
    struct PhysicsLayer {
        uint32_t layerId;
        std::string name;
        int32_t bitValue;
        int32_t collidesWith = 0;
        bool collidesWithSelf = true;

        bool IsValid() const {
            return !name.empty() && bitValue > 0;
        }
    };

    class PhysicsLayerManager {
        private:
            static std::vector<PhysicsLayer> layers;
            static std::vector<std::string> layerNames;
            static PhysicsLayer nullLayer;

            static uint32_t GetNextLayerID();

        public:
            static uint32_t AddLayer(const std::string& name, bool setCollisions = true);
            static void RemoveLayer(uint32_t layerId);

            static void UpdateLayerName(uint32_t layerId, const std::string& newName);

            static void SetLayerCollision(uint32_t layerId, uint32_t otherLayer, bool shouldCollide);
            static std::vector<PhysicsLayer> GetLayerCollisions(uint32_t layerId);

            static const std::vector<PhysicsLayer>& GetLayers() { return layers; }
            static const std::vector<std::string>& GetLayerNames() { return layerNames; }

            static PhysicsLayer& GetLayer(uint32_t layerId);
            static PhysicsLayer& GetLayer(const std::string& layerName);
            static uint32_t GetLayerCount() { return uint32_t(layers.size()); }

            static bool ShouldCollide(uint32_t layer1, uint32_t layer2);
            static bool IsLayerValid(uint32_t layerId);
            static bool IsLayerValid(const std::string& layerName);

            static void ClearLayers();
    };
}