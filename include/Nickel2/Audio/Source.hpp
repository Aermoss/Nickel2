#pragma once

#include <cstdint>

#include <al/al.h>
#include <al/alc.h>
#include <al/alut.h>

#include "Listener.hpp"

namespace Nickel2 {
    class Entity;
    class Listener;

    class Source {
        private:
            Entity* entity;
            Listener* listener;

            bool dirtyVelocity = false;
            glm::vec3 lastTranslation, velocity;
            uint32_t id, buffer, source;
            float rollOffFactor, refDistance, maxDistance;
            float gain, pitch;
            bool looping;

        public:
            Source(Entity* entity, Listener* listener, const char* filePath, bool looping = false, \
                float gain = 1.0f, float pitch = 1.0f, float rollOffFactor = 1.0f, float refDistance = 1.0f, float maxDistance = FLT_MAX);
            ~Source();

            void SetLooping(bool looping);
            void SetPitch(float pitch);
            void SetGain(float gain);
            void SetRollOffFactor(float rollOffFactor);
            void SetRefDistance(float refDistance);
            void SetMaxDistance(float maxDistance);

            bool GetLooping();
            float GetPitch();
            float GetGain();
            float GetRollOffFactor();
            float GetRefDistance();
            float GetMaxDistance();

            void Play();
            void Pause();
            void Stop();

            bool Playing();
            bool Paused();
            bool Stopped();

            void SetVelocity(const glm::vec3& velocity);
            glm::vec3 GetVelocity();
            void ResetVelocity();

            void Update(float deltaTime);
            void Destroy();
    };
}