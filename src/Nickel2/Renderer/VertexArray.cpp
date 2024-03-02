#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/VertexArray.hpp>

namespace Nickel2 {
    VertexArray::VertexArray() {
        glGenVertexArrays(1, &id);
    }

    VertexArray::~VertexArray() {

    }

    void VertexArray::bind() {
        glBindVertexArray(id);
    }

    void VertexArray::unbind() {
        glBindVertexArray(0);
    }

    void VertexArray::defineAttrib(uint32_t index, uint32_t size, uint32_t stride, const void* pointer, uint32_t type, bool normalized) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    }

    void VertexArray::destroy() {
        glDeleteVertexArrays(1, &id);
    }
}