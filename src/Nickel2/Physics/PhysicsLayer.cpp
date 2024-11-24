#include <Nickel2/nkpch.hpp>
#include <Nickel2/Physics/PhysicsLayer.hpp>

namespace Nickel2 {
    template<typename T, typename ConditionFunction> inline bool RemoveIf(std::vector<T>& vector, ConditionFunction condition) {
        for (auto it = vector.begin(); it != vector.end(); it++) {
            if (condition(*it)) {
                vector.erase(it);
                return true;
            }
        } return false;
    }

    uint32_t PhysicsLayerManager::AddLayer(const std::string& name, bool setCollisions) {
        for (const auto& layer : layers) {
            if (layer.name == name)
                return layer.layerId;
        }

        uint32_t layerId = GetNextLayerID();
        PhysicsLayer layer = { layerId, name, static_cast<int32_t>(BIT(layerId)), static_cast<int32_t>(BIT(layerId)) };
        layers.insert(layers.begin() + layerId, layer);
        layerNames.insert(layerNames.begin() + layerId, name);

        if (setCollisions) {
            for (const auto& layer2 : layers)
                SetLayerCollision(layer.layerId, layer2.layerId, true);
        } return layer.layerId;
    }

    void PhysicsLayerManager::RemoveLayer(uint32_t layerId) {
        PhysicsLayer& layerInfo = GetLayer(layerId);

        for (auto& otherLayer : layers) {
            if (otherLayer.layerId == layerId)
                continue;

            if (otherLayer.collidesWith & layerInfo.bitValue)
                otherLayer.collidesWith &= ~layerInfo.bitValue;
        }

        RemoveIf(layerNames, [&](const std::string& name) { return name == layerInfo.name; });
        RemoveIf(layers, [&](const PhysicsLayer& layer) { return layer.layerId == layerId; });
    }

    void PhysicsLayerManager::UpdateLayerName(uint32_t layerId, const std::string& newName) {
        for (const auto& layerName : layerNames)
            if (layerName == newName) return;

        PhysicsLayer& layer = GetLayer(layerId);
        RemoveIf(layerNames, [&](const std::string& name) { return name == layer.name; });
        layerNames.insert(layerNames.begin() + layerId, newName);
        layer.name = newName;
    }

    void PhysicsLayerManager::SetLayerCollision(uint32_t layerId, uint32_t otherLayer, bool shouldCollide) {
        if (ShouldCollide(layerId, otherLayer) && shouldCollide)
            return;

        PhysicsLayer& layerInfo = GetLayer(layerId);
        PhysicsLayer& otherLayerInfo = GetLayer(otherLayer);

        if (shouldCollide) {
            layerInfo.collidesWith |= otherLayerInfo.bitValue;
            otherLayerInfo.collidesWith |= layerInfo.bitValue;
        } else {
            layerInfo.collidesWith &= ~otherLayerInfo.bitValue;
            otherLayerInfo.collidesWith &= ~layerInfo.bitValue;
        }
    }

    std::vector<PhysicsLayer> PhysicsLayerManager::GetLayerCollisions(uint32_t layerId) {
        const PhysicsLayer& layer = GetLayer(layerId);
        std::vector<PhysicsLayer> layers;

        for (const auto& otherLayer : layers) {
            if (otherLayer.layerId == layerId)
                continue;

            if (layer.collidesWith & otherLayer.bitValue)
                layers.push_back(otherLayer);
        }

        return layers;
    }

    PhysicsLayer& PhysicsLayerManager::GetLayer(uint32_t layerId) {
        return layerId >= layers.size() ? nullLayer : layers[layerId];
    }

    PhysicsLayer& PhysicsLayerManager::GetLayer(const std::string& layerName) {
        for (auto& layer : layers) {
            if (layer.name == layerName)
                return layer;
        } return nullLayer;
    }

    bool PhysicsLayerManager::ShouldCollide(uint32_t layer1, uint32_t layer2) {
        return GetLayer(layer1).collidesWith & GetLayer(layer2).bitValue;
    }

    bool PhysicsLayerManager::IsLayerValid(uint32_t layerId) {
        const PhysicsLayer& layer = GetLayer(layerId);
        return layer.layerId != nullLayer.layerId && layer.IsValid();
    }

    bool PhysicsLayerManager::IsLayerValid(const std::string& layerName) {
        const PhysicsLayer& layer = GetLayer(layerName);
        return layer.layerId != nullLayer.layerId && layer.IsValid();
    }

    void PhysicsLayerManager::ClearLayers() {
        layers.clear();
        layerNames.clear();
    }

    uint32_t PhysicsLayerManager::GetNextLayerID() {
        int32_t lastId = -1;

        for (const auto& layer : layers) {
            if (lastId != -1 && int32_t(layer.layerId) != lastId + 1)
                return uint32_t(lastId + 1);

            lastId = layer.layerId;
        } return (uint32_t) layers.size();
    }

    std::vector<PhysicsLayer> PhysicsLayerManager::layers;
    std::vector<std::string> PhysicsLayerManager::layerNames;
    PhysicsLayer PhysicsLayerManager::nullLayer = { (uint32_t) -1, "NULL", -1, -1};
}