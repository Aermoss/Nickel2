#pragma once

#include <Nickel2/Physics/PhysicsLayer.hpp>

#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

namespace Nickel2 {
    class JoltLayerInterface : public JPH::BroadPhaseLayerInterface {
        private:
            std::vector<JPH::BroadPhaseLayer> broadPhaseLayers;

        public:
            JoltLayerInterface();
            virtual ~JoltLayerInterface() = default;

            virtual uint32_t GetNumBroadPhaseLayers() const override { return static_cast<uint32_t>(broadPhaseLayers.size()); }
            virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;
    };
}