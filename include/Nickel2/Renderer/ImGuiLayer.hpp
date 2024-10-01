#pragma once

#include "../Core/Layer.hpp"

namespace Nickel2 {
    class ImGuiLayer : public Layer {
        public:
            virtual void Begin() = 0;
            virtual void End() = 0;

            static ImGuiLayer* Create();
    };
}