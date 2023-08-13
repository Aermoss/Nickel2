#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <glm/glm.hpp>

#include <nickel2/audio/source.hpp>
#include <nickel2/camera.hpp>

namespace nickel2::audio {
    class Listener {
        private:
            std::unordered_map <uint32_t, Source*> sources;
            Camera* camera;

            uint32_t genUniqueSourceID();

        public:
            Transform* transform;

            Listener(Camera* camera = nullptr);
            ~Listener();

            uint32_t addSource(Source* source);
            void removeSource(uint32_t id);

            void setCamera(Camera* camera);
            Camera* getCamera();

            void update();
            void destroy();
    };
}