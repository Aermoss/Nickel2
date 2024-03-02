#pragma once

#include <cstdint>

#include <al/al.h>
#include <al/alc.h>
#include <al/alut.h>

#include "../Scene/Transform.hpp"

#include "Listener.hpp"

namespace Nickel2 {
    class Listener;

    class Source {
        private:
            Listener* listener;
            uint32_t id, buffer, source;
            float pitch, gain;
            bool looping;

        public:
            Transform* transform;

            Source(Listener* listener, Transform* transform, const char* filePath, bool looping = false, float pitch = 1.0f, float gain = 1.0f);
            ~Source();

            void setLooping(bool looping);
            void setPitch(float pitch);
            void setGain(float gain);

            bool getLooping();
            float getPitch();
            float getGain();

            void play();
            void pause();
            void stop();

            bool playing();
            bool paused();
            bool stopped();

            void update();
            void destroy();
    };
}