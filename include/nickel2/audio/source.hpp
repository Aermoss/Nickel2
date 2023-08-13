#pragma once

#include <stdint.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include <nickel2/audio/listener.hpp>
#include <nickel2/transform.hpp>

namespace nickel2::audio {
    class Listener;

    class Source {
        private:
            Listener* listener;
            uint32_t id, buffer, source;
            float pitch, gain;
            bool looping;

        public:
            Transform* transform;

            Source(Listener* listener, const char* filePath, bool looping = false, float pitch = 1.0f, float gain = 1.0f);
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