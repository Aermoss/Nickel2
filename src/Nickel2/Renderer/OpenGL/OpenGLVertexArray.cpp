#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/OpenGL/OpenGLVertexArray.hpp>

#include <glad/glad.h>

namespace Nickel2 {
    namespace Utils {
        static GLenum GetDataType(DataType type) {
            switch (type) {
                case DataType::Float: return GL_FLOAT;
                case DataType::Float2: return GL_FLOAT;
                case DataType::Float3: return GL_FLOAT;
                case DataType::Float4: return GL_FLOAT;
                case DataType::Mat3: return GL_FLOAT;
                case DataType::Mat4: return GL_FLOAT;
                case DataType::Int: return GL_INT;
                case DataType::Int2: return GL_INT;
                case DataType::Int3: return GL_INT;
                case DataType::Int4: return GL_INT;
                case DataType::Bool: return GL_BOOL;
                default: break;
            } return 0;
        }
    }

    OpenGLVertexArray::OpenGLVertexArray() {
        glCreateVertexArrays(1, &id);
    }

    OpenGLVertexArray::~OpenGLVertexArray() {
        glDeleteVertexArrays(1, &id);
    }

    void OpenGLVertexArray::Bind() const {
        glBindVertexArray(id);
    }

    void OpenGLVertexArray::Unbind() const {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
        const auto& layout = vertexBuffer->GetLayout();

        for (const auto& element : layout) {
            switch (element.type) {
                case DataType::Float:
                case DataType::Float2:
                case DataType::Float3:
                case DataType::Float4: {
                    glEnableVertexArrayAttrib(id, vertexBufferIndex);
                    glVertexArrayAttribBinding(id, vertexBufferIndex, 0);
                    glVertexArrayAttribFormat(id, vertexBufferIndex, element.GetComponentCount(), \
                        Utils::GetDataType(element.type), element.normalized ? GL_TRUE : GL_FALSE, element.offset);
                    vertexBufferIndex++;
                    break;
                }

                case DataType::Int:
                case DataType::Int2:
                case DataType::Int3:
                case DataType::Int4:
                case DataType::Bool: {
                    glEnableVertexArrayAttrib(id, vertexBufferIndex);
                    glVertexArrayAttribBinding(id, vertexBufferIndex, 0);
                    glVertexArrayAttribIFormat(id, vertexBufferIndex, element.GetComponentCount(), \
                        Utils::GetDataType(element.type), element.offset);
                    vertexBufferIndex++;
                    break;
                }

                case DataType::Mat3:
                case DataType::Mat4: {
                    uint8_t count = element.GetComponentCount();

                    for (uint8_t i = 0; i < count; i++) {
                        glEnableVertexArrayAttrib(id, vertexBufferIndex);
                        glVertexArrayAttribBinding(id, vertexBufferIndex, 0);
                        glVertexArrayBindingDivisor(id, vertexBufferIndex, 1);
                        glVertexArrayAttribFormat(id, vertexBufferIndex, count, Utils::GetDataType(element.type), \
                            element.normalized ? GL_TRUE : GL_FALSE, element.offset + sizeof(float) * count * i);
                        vertexBufferIndex++;
                    } break;
                }

                default:
                    break;
            }
        }

        glVertexArrayVertexBuffer(id, 0, vertexBuffer->GetID(), 0, layout.GetStride());
        vertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
        glVertexArrayElementBuffer(id, indexBuffer->GetID());
        this->indexBuffer = indexBuffer;
    }
}