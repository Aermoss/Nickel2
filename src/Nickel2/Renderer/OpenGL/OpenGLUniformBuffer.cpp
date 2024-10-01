#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/OpenGL/OpenGLUniformBuffer.hpp>

#include <glad/glad.h>

namespace Nickel2 {
    OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding) {
        glCreateBuffers(1, &id);
        glNamedBufferData(id, size, nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, id);
    }

    OpenGLUniformBuffer::~OpenGLUniformBuffer() {
        glDeleteBuffers(1, &id);
    }

    void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset) {
        glNamedBufferSubData(id, offset, size, data);
    }
}