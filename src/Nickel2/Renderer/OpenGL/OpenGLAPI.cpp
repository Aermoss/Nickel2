#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/OpenGL/OpenGLAPI.hpp>

#include <glad/glad.h>

namespace Nickel2 {
    namespace Utils {
        const char* GetSource(int source) {
            switch (source) {
                case GL_DEBUG_SOURCE_API: return "RendererAPIType";
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "Window System";
                case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
                case GL_DEBUG_SOURCE_THIRD_PARTY: return "Third Party";
                case GL_DEBUG_SOURCE_APPLICATION: return "Application";
                case GL_DEBUG_SOURCE_OTHER: return "Other";
                default: return "Unknown";
            }
        }

        const char* GetType(int type) {
            switch (type) {
                case GL_DEBUG_TYPE_ERROR: return "Error";
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behavior";
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "Undefined Behavior";
                case GL_DEBUG_TYPE_PORTABILITY: return "Portability";
                case GL_DEBUG_TYPE_PERFORMANCE: return "Performance";
                case GL_DEBUG_TYPE_MARKER: return "Marker";
                case GL_DEBUG_TYPE_PUSH_GROUP: return "Push Group";
                case GL_DEBUG_TYPE_POP_GROUP: return "Pop Group";
                case GL_DEBUG_TYPE_OTHER: return "Other";
                default: return "Unknown";
            }
        }
        
        const char* GetSeverity(int severity) {
            switch (severity) {
                case GL_DEBUG_SEVERITY_HIGH: return "High";
                case GL_DEBUG_SEVERITY_MEDIUM: return "Medium";
                case GL_DEBUG_SEVERITY_LOW: return "Low";
                case GL_DEBUG_SEVERITY_NOTIFICATION: return "Notification";
                default: return "Unknown";
            }
        }

        void APIENTRY MessageCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int length, const char* message, const void* userParam) {
            if (severity != GL_DEBUG_SEVERITY_NOTIFICATION && !(type == GL_DEBUG_TYPE_OTHER && severity == GL_DEBUG_SEVERITY_LOW)) {
                std::string string = std::string(message);
                string = string[string.size() - 1] != '.' ? string + "." : string;
                Logger::Log(type != GL_DEBUG_TYPE_ERROR ? \
                    (severity != GL_DEBUG_SEVERITY_NOTIFICATION ? Logger::Level::Warn : Logger::Level::Info) : Logger::Level::Error, "OpenGL", string.c_str());
            }
        }
    }

    void OpenGLRendererAPI::Initialize() {
#if defined(NK_DEBUG)
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(Utils::MessageCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LINE_SMOOTH);
    }

    void OpenGLRendererAPI::Terminate() {

    }

    void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount) {
        vertexArray->Bind();
        uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLRendererAPI::DrawLines(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount) {
        vertexArray->Bind();
        glDrawArrays(GL_LINES, 0, vertexCount);
    }

    void OpenGLRendererAPI::SetLineWidth(float width) {
        glLineWidth(width);
    }
}