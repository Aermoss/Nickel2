#pragma once

#include <memory>
#include <cstdint>
#include <cfloat>

namespace Nickel2 {
    class Entity;

    class AudioSource {
        protected:
            Entity* entity;

        private:
            virtual void Update(float deltaTime) = 0;

        public:
            AudioSource(Entity* entity) : entity(entity) {}
            virtual ~AudioSource() = default;

            virtual void SetLooping(bool looping) = 0;
            virtual void SetPitch(float pitch) = 0;
            virtual void SetGain(float gain) = 0;
            virtual void SetRollOffFactor(float rollOffFactor) = 0;
            virtual void SetRefDistance(float refDistance) = 0;
            virtual void SetMaxDistance(float maxDistance) = 0;

            virtual bool GetLooping() = 0;
            virtual float GetPitch() = 0;
            virtual float GetGain() = 0;
            virtual float GetRollOffFactor() = 0;
            virtual float GetRefDistance() = 0;
            virtual float GetMaxDistance() = 0;

            virtual void Play() = 0;
            virtual void Pause() = 0;
            virtual void Stop() = 0;

            virtual bool IsPlaying() = 0;
            virtual bool IsPaused() = 0;
            virtual bool IsStopped() = 0;

            static std::shared_ptr<AudioSource> Create(Entity* entity, const char* filePath, bool looping = false, float gain = 1.0f, float pitch = 1.0f, float rollOffFactor = 1.0f, float refDistance = 1.0f, float maxDistance = FLT_MAX);

            friend class AudioSystem;
    };
}