#include <nickel2/buffer.hpp>

namespace nickel2 {
    Buffer::Buffer(uint32_t type) : type(type) {
        glGenBuffers(1, &id);
    }

    Buffer::~Buffer() {

    }

    void Buffer::bind() {
        glBindBuffer(type, id);
    }

    void Buffer::unbind() {
        glBindBuffer(type, 0);
    }

    void Buffer::bufferData(uint32_t size, const void* data, uint32_t usage) {
        glBufferData(type, size, data, usage);
    }

    void Buffer::destroy() {
        glDeleteBuffers(1, &id);
    }
}