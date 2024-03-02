#pragma once

#include <cstdint>

#include <al/al.h>
#include <al/alc.h>
#include <al/alut.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Renderer/Camera.hpp"

#include "Source.hpp"

namespace Nickel2 {
    class Source;
    class Camera;
    
    class Listener {
        private:
            std::unordered_map<uint32_t, Source*> sources;
            Camera* camera;

            uint32_t genUniqueSourceID();

        public:
            Transform* transform;

            Listener(Camera* camera, Transform* transform);
            ~Listener();

            uint32_t addSource(Source* source);
            void removeSource(uint32_t id);

            void setCamera(Camera* camera);
            Camera* getCamera();

            void update();
            void destroy();
    };
}