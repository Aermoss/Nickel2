#pragma once

#include <fstream>
#include <string>

#include "../Renderer/Buffer.hpp"

namespace Nickel2::Utils {
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

    bool IsFileExists(const char* filePath);

    void WriteFileBinary(const char* filePath, Buffer& data);
    void WriteFileBinary(const char* filePath, const std::vector<uint32_t>& data);
    void WriteFile(const char* filePath, const std::string& data);

    Buffer ReadFileBinary(const char* filePath);
    void ReadFileBinary(const char* filePath, std::vector<uint32_t>& data);
    std::string ReadFile(const char* filePath);
}