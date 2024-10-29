#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/OpenGL/OpenGLContext.hpp>

#include <glfw/glfw3.h>
#include <glad/glad.h>

namespace Nickel2 {
    namespace Utils {
        const char* GetErrorString(int32_t error) {
            switch (error) {
                case GL_NO_ERROR: return "No error.";
                case GL_INVALID_ENUM: return "Invalid enum.";
                case GL_INVALID_VALUE: return "Invalid value.";
                case GL_INVALID_OPERATION: return "Invalid operation.";
                case GL_INVALID_FRAMEBUFFER_OPERATION: return "Invalid framebuffer operation.";
                case GL_OUT_OF_MEMORY: return "Out of memory.";
                case GL_STACK_UNDERFLOW: return "Stack underflow.";
                case GL_STACK_OVERFLOW: return "Stack overflow.";
                case GL_CONTEXT_LOST: return "Context lost.";
                default: return "Unknown error.";
            }
        }
    }

    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : windowHandle(windowHandle) {
        NK_CORE_ASSERT(windowHandle, "Window handle is null.");
    }

    void OpenGLContext::Initialize() {
        glfwMakeContextCurrent(windowHandle);
        int32_t status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        NK_CORE_ASSERT(status, "Failed to initialize GLAD.");

        int32_t versionMajor, versionMinor;
        glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
        glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
        NK_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Unsupported GPU.");
    }

    void OpenGLContext::Terminate() {

    }

    void OpenGLContext::CheckErrors() {
        /* int32_t error = glGetError();
        NK_CORE_ASSERT(!error, Utils::GetErrorString(error)); */
    }

    void OpenGLContext::SwapBuffers() {
        glfwSwapBuffers(windowHandle);
    }
}