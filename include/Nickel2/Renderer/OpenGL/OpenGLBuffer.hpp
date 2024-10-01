#pragma once

#include <Nickel2/Renderer/Buffer.hpp>

namespace Nickel2 {
    class OpenGLVertexBuffer : public VertexBuffer {
        private:
            uint32_t id;
            BufferLayout layout;

        public:
            OpenGLVertexBuffer(uint32_t size);
            OpenGLVertexBuffer(float* vertices, uint32_t size);
            virtual ~OpenGLVertexBuffer();

            virtual uint32_t GetID() const { return id; }

            virtual void Bind() const override;
            virtual void Unbind() const override;
            
            virtual void SetData(const void* data, uint32_t size) override;

            virtual const BufferLayout& GetLayout() const override { return this->layout; }
            virtual void SetLayout(const BufferLayout& layout) override { this->layout = layout; }
    };

    class OpenGLIndexBuffer : public IndexBuffer {
        private:
            uint32_t id, count;

        public:
            OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
            virtual ~OpenGLIndexBuffer();

            virtual uint32_t GetID() const { return id; }
            virtual uint32_t GetCount() const { return count; }

            virtual void Bind() const;
            virtual void Unbind() const;
    };
}