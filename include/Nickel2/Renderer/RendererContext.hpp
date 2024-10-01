#pragma once

#include <Nickel2/Core/Base.hpp>

#include <glfw/glfw3.h>

namespace Nickel2 {
    class RendererContext {
        public:
            virtual ~RendererContext() = default;

            virtual void Initialize() = 0;
            virtual void Terminate() = 0;
            virtual void CheckErrors() = 0;
            virtual void SwapBuffers() = 0;

            static std::unique_ptr<RendererContext> Create(void* window);
    };
}