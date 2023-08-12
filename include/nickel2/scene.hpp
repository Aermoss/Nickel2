#pragma once

#include <stdint.h>
#include <vector>

#include "model.hpp"
#include "light.hpp"

namespace nickel2 {
    class Scene {
        private:
            std::vector <Model*> objects;

        public:
            std::vector <Light> lights;

            Scene();
            ~Scene();

            std::vector <Model*>& getObjects();
            std::vector <Light>& getLights();

            void submit(Model* object);
            void submit(Light light);
            void destroy();
    };
}