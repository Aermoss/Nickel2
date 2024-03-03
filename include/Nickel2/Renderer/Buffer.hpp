#pragma once

#include <cstdint>
#include <cstring>

#include <glad/glad.h>

#include "../Core/Constants.hpp"

namespace Nickel2 {
    namespace ext {
        struct Buffer {
            uint8_t* data = nullptr;
            uint64_t size = 0;

            Buffer() = default;
            Buffer(uint64_t size) { allocate(size); }
            Buffer(const Buffer&) = default;

            static Buffer copy(Buffer other) {
                Buffer result(other.size);
                memcpy(result.data, other.data, other.size);
                return result;
            }

            void allocate(uint64_t size) {
                delete[] data;
                this->data = new uint8_t[size];
                this->size = size;
            }

            void release() {
                delete[] data;
                this->data = nullptr;
                this->size = 0;
            }

            template <typename T> T* as() {
                return (T*) data;
            }

            operator bool() const {
                return (bool) data;
            }
        };

        struct ScopedBuffer {
            private:
                Buffer buffer;

            public:
                ScopedBuffer(Buffer buffer) : buffer(buffer) {}
                ScopedBuffer(uint64_t size) : buffer(size) {}
                ~ScopedBuffer() { buffer.release(); }

                uint8_t* data() { return buffer.data; }
                uint64_t size() { return buffer.size; }

                template <typename T> T* as() {
                    return buffer.as<T>();
                }

                operator bool() const {
                    return buffer;
                }
        };
    }

    class Buffer {
        private:
            uint32_t id, type;

        public:
            Buffer(uint32_t type);
            ~Buffer();

            void bind();
            void unbind();
            void bufferData(uint32_t size, const void* data, uint32_t usage = GL_STATIC_DRAW);
            void destroy();
    };
}