#pragma once

#include <Nickel2/Core/Buffer.hpp>

#include <Jolt/Core/StreamIn.h>
#include <Jolt/Core/StreamOut.h>

namespace Nickel2 {
    class JoltBinaryStreamReader : public JPH::StreamIn {
        public:
            JoltBinaryStreamReader(const Buffer& buffer) : buffer(&buffer) {}

            ~JoltBinaryStreamReader() {
                buffer = nullptr;
                readBytes = 0;
            }

            virtual void ReadBytes(void* outData, size_t inNumBytes) override {
                memcpy(outData, ((byte*) buffer->data) + readBytes, inNumBytes);
                readBytes += inNumBytes;
            }

            virtual bool IsEOF() const override {
                return buffer == nullptr || readBytes > buffer->size;
            }

            virtual bool IsFailed() const override {
                return buffer == nullptr || buffer->data == nullptr || buffer->size == 0;
            }

        private:
            const Buffer* buffer = nullptr;
            size_t readBytes = 0;
    };

    class JoltBinaryStreamWriter : public JPH::StreamOut {
        public:
            void WriteBytes(const void* inData, size_t inNumBytes) override {
                size_t currentOffset = tempBuffer.size();
                tempBuffer.resize(currentOffset + inNumBytes);
                memcpy(tempBuffer.data() + currentOffset, inData, inNumBytes);
            }

            bool IsFailed() const override {
                return false;
            }

            Buffer ToBuffer() const {
                return Buffer::Copy(tempBuffer.data(), (uint64_t) tempBuffer.size());
            }

        private:
            std::vector<byte> tempBuffer;
    };
}