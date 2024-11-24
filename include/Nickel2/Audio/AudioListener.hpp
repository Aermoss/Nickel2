#pragma once

#include <memory>
#include <cstdint>

namespace Nickel2 {
    class Entity;

    enum class DistanceModel : uint32_t {
        None = 0, InverseDistance, InverseDistanceClamped, LinearDistance, LinearDistanceClamped, ExponentDistance, ExponentDistanceClamped
    };
    
    class AudioListener {
        protected:
            Entity* entity;

        private:
            virtual void Update(float deltaTime) = 0;

        public:
            AudioListener(Entity* entity) : entity(entity) {}
            virtual ~AudioListener() = default;

            virtual void SetDistanceModel(DistanceModel distanceModel) = 0;
            virtual DistanceModel GetDistanceModel() = 0;

            static std::shared_ptr<AudioListener> Create(Entity* entity, DistanceModel distanceModel = DistanceModel::InverseDistance);

            friend class AudioSystem;
    };
}