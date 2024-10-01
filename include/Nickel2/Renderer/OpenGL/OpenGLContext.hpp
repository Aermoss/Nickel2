#pragma once

#include <Nickel2/Renderer/RendererContext.hpp>

struct GLFWwindow;

namespace Nickel2 {
    class OpenGLContext : public RendererContext {
        private:
            GLFWwindow* windowHandle;

        public:
            OpenGLContext(GLFWwindow* windowHandle);

            virtual void Initialize() override;
            virtual void Terminate() override;
            virtual void CheckErrors() override;
            virtual void SwapBuffers() override;
    };
}