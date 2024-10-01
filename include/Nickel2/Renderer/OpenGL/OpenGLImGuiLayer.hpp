#pragma once

#include "../ImGuiLayer.hpp"

namespace Nickel2 {
    class OpenGLImGuiLayer : public ImGuiLayer {
        public:
            OpenGLImGuiLayer() {}
            virtual ~OpenGLImGuiLayer() {}

            virtual void Begin() override;
            virtual void End() override;

            virtual void OnAttach() override;
            virtual void OnDetach() override;
    };
}