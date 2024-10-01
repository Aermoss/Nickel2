#pragma once

#include "Base.hpp"
#include "Layer.hpp"

#include <vector>

namespace Nickel2 {
    class LayerStack {
        private:
            std::vector<Layer*> layers;
            uint32_t layerInsertIndex = 0;
            
        public:
            LayerStack() {}
            ~LayerStack() {}

            void PushLayer(Layer* layer);
            void PopLayer(Layer* layer);

            std::vector<Layer*>::iterator begin() { return layers.begin(); }
            std::vector<Layer*>::iterator end() { return layers.end(); }
    };
}