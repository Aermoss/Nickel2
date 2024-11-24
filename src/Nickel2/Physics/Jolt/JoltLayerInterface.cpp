#include <Nickel2/nkpch.hpp>
#include <Nickel2/Physics/Jolt/JoltLayerInterface.hpp>

namespace Nickel2 {
    JoltLayerInterface::JoltLayerInterface() {
        uint32_t layerCount = PhysicsLayerManager::GetLayerCount();
        broadPhaseLayers.resize(layerCount);

        for (const auto& layer : PhysicsLayerManager::GetLayers())
            broadPhaseLayers[layer.layerId] = JPH::BroadPhaseLayer(layer.layerId);
    }

    JPH::BroadPhaseLayer JoltLayerInterface::GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const {
        return broadPhaseLayers[inLayer];
    }
}