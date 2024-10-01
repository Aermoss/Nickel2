#pragma once

#include "../Event/Event.hpp"

#include <string>

namespace Nickel2 {
    class Layer {
        public:
            virtual ~Layer() {}

            virtual void OnAttach() {}
            virtual void OnDetach() {}

            virtual void OnUpdate(float deltaTime) {}
            virtual void OnRender(float deltaTime) {}
            virtual void OnEvent(Event& event) {}
    };
}