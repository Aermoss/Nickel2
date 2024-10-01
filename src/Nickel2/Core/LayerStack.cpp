#include <Nickel2/nkpch.hpp>
#include <Nickel2/Core/LayerStack.hpp>

namespace Nickel2 {
    void LayerStack::PushLayer(Layer* layer) {
        layers.push_back(layer);
    }

    void LayerStack::PopLayer(Layer* layer) {
        std::vector<Layer*>::iterator result = std::find(layers.begin(), layers.end(), layer);
        if (result != layers.end()) layers.erase(result);
    }
}