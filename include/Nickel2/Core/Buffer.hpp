#pragma once

namespace Nickel2 {
    struct Buffer {
        uint8_t* data = nullptr;
        uint64_t size = 0;

        Buffer() = default;
        Buffer(uint64_t size) { Allocate(size); }
        Buffer(const Buffer&) = default;

        static Buffer Copy(Buffer other) {
            Buffer result(other.size);
            memcpy(result.data, other.data, other.size);
            return result;
        }

        static Buffer Copy(const void* data, uint64_t size) {
            Buffer buffer;
            buffer.Allocate(size);
            memcpy(buffer.data, data, size);
            return buffer;
        }

        void Allocate(uint64_t size) {
            delete[] data;
            this->data = new uint8_t[size];
            this->size = size;
        }

        void Release() {
            delete[] data;
            this->data = nullptr;
            this->size = 0;
        }

        template<typename T> T* As() {
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
            ~ScopedBuffer() { buffer.Release(); }

            uint8_t* Data() { return buffer.data; }
            uint64_t Size() { return buffer.size; }

            template<typename T> T* As() {
                return buffer.As<T>();
            }

            operator bool() const {
                return buffer;
            }
    };
}