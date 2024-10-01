#pragma once

#include <cstdint>

#include <al/al.h>
#include <al/alc.h>
#include <al/alut.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Source.hpp"

namespace Nickel2 {
    class Source;
    class Camera;
    class Entity;

    enum class DistanceModel : int32_t {
        None = AL_NONE, InverseDistance = AL_INVERSE_DISTANCE, InverseDistanceClamped = AL_INVERSE_DISTANCE_CLAMPED,
        LinearDistance = AL_LINEAR_DISTANCE, LinearDistanceClamped = AL_LINEAR_DISTANCE_CLAMPED,
        ExponentDistance = AL_EXPONENT_DISTANCE, ExponentDistanceClamped = AL_EXPONENT_DISTANCE_CLAMPED
    };
    
    class Listener {
        private:
            Entity* entity;
            Camera* camera;

            bool dirtyVelocity = false;
            glm::vec3 lastTranslation, velocity;
            std::unordered_map<uint32_t, Source*> sources;

            uint32_t GenUniqueSourceID();

        public:
            Listener(Entity* entity, DistanceModel distanceModel = DistanceModel::InverseDistance);
            ~Listener();

            uint32_t AddSource(Source* source);
            void RemoveSource(uint32_t id);

            void SetDistanceModel(int32_t distanceModel);
            
            void SetVelocity(const glm::vec3& velocity);
            glm::vec3 GetVelocity();
            void ResetVelocity();

            void Update(float deltaTime);
            void Destroy();
    };
}