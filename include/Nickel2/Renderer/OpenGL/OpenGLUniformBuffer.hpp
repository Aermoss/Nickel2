#pragma once

#include <Nickel2/Renderer/UniformBuffer.hpp>

namespace Nickel2 {
    class OpenGLUniformBuffer : public UniformBuffer {
        private:
            uint32_t id = 0;
            
        public:
            OpenGLUniformBuffer(uint32_t size, uint32_t binding);
            virtual ~OpenGLUniformBuffer();

            virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
    };
}