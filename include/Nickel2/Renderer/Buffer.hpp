#pragma once

#include <vector>
#include <cstdint>
#include <memory>
#include <string>

namespace Nickel2 {
    enum class DataType {
        None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
    };

    struct BufferElement {
        std::string name;
        DataType type;
        uint32_t size;
        size_t offset;
        bool normalized;

        BufferElement() = default;

        BufferElement(DataType type, const std::string& name, bool normalized = false)
            : name(name), type(type), size(GetSize()), offset(0), normalized(normalized) {}

        uint32_t GetSize() {
            switch (type) {
                case DataType::Float: return 4;
                case DataType::Float2: return 4 * 2;
                case DataType::Float3: return 4 * 3;
                case DataType::Float4: return 4 * 4;
                case DataType::Mat3: return 4 * 3 * 3;
                case DataType::Mat4: return 4 * 4 * 4;
                case DataType::Int: return 4;
                case DataType::Int2: return 4 * 2;
                case DataType::Int3: return 4 * 3;
                case DataType::Int4: return 4 * 4;
                case DataType::Bool: return 1;
                default: break;
            } return 0;
        }

        uint32_t GetComponentCount() const {
            switch (type) {
                case DataType::Float: return 1;
                case DataType::Float2: return 2;
                case DataType::Float3: return 3;
                case DataType::Float4: return 4;
                case DataType::Mat3: return 3;
                case DataType::Mat4: return 4;
                case DataType::Int: return 1;
                case DataType::Int2: return 2;
                case DataType::Int3: return 3;
                case DataType::Int4: return 4;
                case DataType::Bool: return 1;
                default: break;
            } return 0;
        }
    };

    class BufferLayout {
        private:
            std::vector<BufferElement> elements;
            uint32_t stride = 0;

            void CalculateOffsetsAndStride() {
                size_t offset = 0;
                stride = 0;
                
                for (auto& element : elements) {
                    element.offset = offset;
                    offset += element.size;
                    stride += element.size;
                }
            }

        public:
            BufferLayout() {}

            BufferLayout(std::initializer_list<BufferElement> elements) : elements(elements) {
                CalculateOffsetsAndStride();
            }

            uint32_t GetStride() const { return stride; }
            const std::vector<BufferElement>& GetElements() const { return elements; }

            std::vector<BufferElement>::iterator begin() { return elements.begin(); }
            std::vector<BufferElement>::iterator end() { return elements.end(); }
            std::vector<BufferElement>::const_iterator begin() const { return elements.begin(); }
            std::vector<BufferElement>::const_iterator end() const { return elements.end(); }
    };

    class VertexBuffer {
        public:
            virtual ~VertexBuffer() = default;

            virtual uint32_t GetID() const = 0;

            virtual void Bind() const = 0;
            virtual void Unbind() const = 0;

            virtual void SetData(const void* data, uint32_t size) = 0;

            virtual const BufferLayout& GetLayout() const = 0;
            virtual void SetLayout(const BufferLayout& layout) = 0;

            static std::shared_ptr<VertexBuffer> Create(uint32_t size);
            static std::shared_ptr<VertexBuffer> Create(float* vertices, uint32_t size);
    };

    class IndexBuffer {
        public:
            virtual ~IndexBuffer() = default;

            virtual uint32_t GetID() const = 0;
            virtual uint32_t GetCount() const = 0;

            virtual void Bind() const = 0;
            virtual void Unbind() const = 0;

            static std::shared_ptr<IndexBuffer> Create(uint32_t* indices, uint32_t count);
    };
}