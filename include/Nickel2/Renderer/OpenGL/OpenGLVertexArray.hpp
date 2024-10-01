#pragma once

#include <Nickel2/Renderer/VertexArray.hpp>

namespace Nickel2 {
    class OpenGLVertexArray : public VertexArray {
        private:
            uint32_t id, vertexBufferIndex = 0;
            std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
            std::shared_ptr<IndexBuffer> indexBuffer;

        public:
            OpenGLVertexArray();
            virtual ~OpenGLVertexArray();

            virtual uint32_t GetID() const { return id; }

            virtual void Bind() const override;
            virtual void Unbind() const override;

            virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
            virtual void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

            virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return vertexBuffers; }
            virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return indexBuffer; } 
    };
}