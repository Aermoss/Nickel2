#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/Shader.hpp>

namespace Nickel2 {
    uint32_t Shader::compileShader(uint32_t type, const char* source) {
        Context* context = getContext();
        std::string name;

        uint32_t shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        switch (type) {
            case GL_VERTEX_SHADER: {
                name = "vertex"; break;
            } case GL_FRAGMENT_SHADER: {
                name = "fragment"; break;
            } case GL_GEOMETRY_SHADER: {
                name = "geometry"; break;
            } case GL_TESS_CONTROL_SHADER: {
                name = "tessellation control"; break;
            } case GL_TESS_EVALUATION_SHADER: {
                name = "tessellation evaluation"; break;
            } case GL_COMPUTE_SHADER: {
                name = "compute"; break;
            }
        }

        int32_t status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        if (!status) {
            int32_t length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            char* buffer = (char*) malloc(length * sizeof(char));
            glGetShaderInfoLog(shader, length, nullptr, buffer);
            context->logger->log(NICKEL2_ERROR, (name + " shader compilation failed: " + std::string(buffer) + ".").c_str());
            free((void*) buffer);
            return 0;
        } else {
            context->logger->log(NICKEL2_INFO, (name + " shader compiled successfully, id: " + std::to_string(shader) + ".").c_str());
        } return shader;
    }

    uint32_t Shader::createProgram(const std::string& computeSource) {
        Context* context = getContext();
        uint32_t computeShader = compileShader(GL_COMPUTE_SHADER, computeSource.c_str());
        uint32_t program = glCreateProgram();
        glAttachShader(program, computeShader);
        glLinkProgram(program);
        glDetachShader(program, computeShader);
        glDeleteShader(computeShader);

        int32_t status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);

        if (!status) {
            int32_t length;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
            char* buffer = (char*) malloc(length * sizeof(char));
            glGetProgramInfoLog(program, length, nullptr, buffer);
            context->logger->log(NICKEL2_ERROR, ("shader program linkage failed: " + std::string(buffer) + ".").c_str());
            free((void*) buffer);
            return 0;
        } else {
            context->logger->log(NICKEL2_INFO, ("shader program linked successfully, id: " + std::to_string(program) + ".").c_str());
        } return program;
    }

    uint32_t Shader::createProgram(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource, const std::string& tessCtrlSource, const std::string& tessEvalSource) {
        Context* context = getContext();
        std::vector <uint32_t> shaders;

        if (!vertexSource.empty())
            shaders.push_back(compileShader(GL_VERTEX_SHADER, vertexSource.c_str()));

        if (!fragmentSource.empty())
            shaders.push_back(compileShader(GL_FRAGMENT_SHADER, fragmentSource.c_str()));

        if (!geometrySource.empty())
            shaders.push_back(compileShader(GL_GEOMETRY_SHADER, geometrySource.c_str()));

        if (!tessCtrlSource.empty())
            shaders.push_back(compileShader(GL_TESS_CONTROL_SHADER, tessCtrlSource.c_str()));
        
        if (!tessEvalSource.empty())
            shaders.push_back(compileShader(GL_TESS_EVALUATION_SHADER, tessEvalSource.c_str()));

        uint32_t program = glCreateProgram();

        for (uint32_t shader : shaders)
            glAttachShader(program, shader);
        
        glLinkProgram(program);

        for (uint32_t shader : shaders)
            glDetachShader(program, shader);

        for (uint32_t shader : shaders)
            glDeleteShader(shader);

        int32_t status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);

        if (!status) {
            int32_t length;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
            char* buffer = (char*) malloc(length * sizeof(char));
            glGetProgramInfoLog(program, length, nullptr, buffer);
            context->logger->log(NICKEL2_ERROR, ("shader program linkage failed: " + std::string(buffer) + ".").c_str());
            free((void*) buffer);
            return 0;
        } else {
            context->logger->log(NICKEL2_INFO, ("shader program linked successfully, id: " + std::to_string(program) + ".").c_str());
        } return program;
    }

    Shader::Shader(const std::string& computeSource) {
        id = createProgram(computeSource);
    }

    Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource, const std::string& tessCtrlSource, const std::string& tessEvalSource) {
        id = createProgram(vertexSource, fragmentSource, geometrySource, tessCtrlSource, tessEvalSource);
    }

    Shader::~Shader() {

    }

    void Shader::use() {
        glUseProgram(id);
    }

    void Shader::unuse() {
        glUseProgram(0);
    }

    void Shader::reload(const std::string& computeSource) {
        glDeleteProgram(id);
        id = createProgram(computeSource);
    }

    void Shader::reload(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource, const std::string& tessCtrlSource, const std::string& tessEvalSource) {
        glDeleteProgram(id);
        id = createProgram(vertexSource, fragmentSource, geometrySource, tessCtrlSource, tessEvalSource);
    }

    void Shader::destroy() {
        glDeleteProgram(id);
        context->logger->log(NICKEL2_INFO, ("shader program deleted successfully, id: " + std::to_string(id)).c_str());
    }

    int32_t Shader::getUniformLocation(const std::string& name) {
        int32_t location = glGetUniformLocation(id, name.c_str());
        // if (location == -1) std::cout << name.c_str() << std::endl;
        return location;
    }

    void Shader::setUniform1i(const std::string& name, int32_t x) {
        glUniform1i(getUniformLocation(name), x);
    }

    void Shader::setUniform1ui(const std::string& name, uint32_t x) {
        glUniform1ui(getUniformLocation(name), x);
    }

    void Shader::setUniform2i(const std::string& name, int32_t x, int32_t y) {
        glUniform2i(getUniformLocation(name), x, y);
    }

    void Shader::setUniform2ui(const std::string& name, uint32_t x, uint32_t y) {
        glUniform2ui(getUniformLocation(name), x, y);
    }

    void Shader::setUniform3i(const std::string& name, int32_t x, int32_t y, int32_t z) {
        glUniform3i(getUniformLocation(name), x, y, z);
    }

    void Shader::setUniform3ui(const std::string& name, uint32_t x, uint32_t y, uint32_t z) {
        glUniform3ui(getUniformLocation(name), x, y, z);
    }

    void Shader::setUniform4i(const std::string& name, int32_t x, int32_t y, int32_t z, int32_t w) {
        glUniform4i(getUniformLocation(name), x, y, z, w);
    }

    void Shader::setUniform4ui(const std::string& name, uint32_t x, uint32_t y, uint32_t z, uint32_t w) {
        glUniform4ui(getUniformLocation(name), x, y, z, w);
    }

    void Shader::setUniform1iv(const std::string& name, int32_t* value, int32_t count) {
        glUniform1iv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform1uiv(const std::string& name, uint32_t* value, int32_t count) {
        glUniform1uiv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform2iv(const std::string& name, int32_t* value, int32_t count) {
        glUniform2iv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform2uiv(const std::string& name, uint32_t* value, int32_t count) {
        glUniform2uiv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform3iv(const std::string& name, int32_t* value, int32_t count) {
        glUniform3iv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform3uiv(const std::string& name, uint32_t* value, int32_t count) {
        glUniform3uiv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform4iv(const std::string& name, int32_t* value, int32_t count) {
        glUniform4iv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform4uiv(const std::string& name, uint32_t* value, int32_t count) {
        glUniform4uiv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform1f(const std::string& name, float x) {
        glUniform1f(getUniformLocation(name), x);
    }

    void Shader::setUniform2f(const std::string& name, float x, float y) {
        glUniform2f(getUniformLocation(name), x, y);
    }

    void Shader::setUniform3f(const std::string& name, float x, float y, float z) {
        glUniform3f(getUniformLocation(name), x, y, z);
    }

    void Shader::setUniform4f(const std::string& name, float x, float y, float z, float w) {
        glUniform4f(getUniformLocation(name), x, y, z, w);
    }

    void Shader::setUniform1fv(const std::string& name, float* value, int32_t count) {
        glUniform1fv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform2fv(const std::string& name, float* value, int32_t count) {
        glUniform2fv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform3fv(const std::string& name, float* value, int32_t count) {
        glUniform3fv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform4fv(const std::string& name, float* value, int32_t count) {
        glUniform4fv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform1d(const std::string& name, double x) {
        glUniform1d(getUniformLocation(name), x);
    }

    void Shader::setUniform2d(const std::string& name, double x, double y) {
        glUniform2d(getUniformLocation(name), x, y);
    }

    void Shader::setUniform3d(const std::string& name, double x, double y, double z) {
        glUniform3d(getUniformLocation(name), x, y, z);
    }

    void Shader::setUniform4d(const std::string& name, double x, double y, double z, double w) {
        glUniform4d(getUniformLocation(name), x, y, z, w);
    }

    void Shader::setUniform1dv(const std::string& name, double* value, int32_t count) {
        glUniform1dv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform2dv(const std::string& name, double* value, int32_t count) {
        glUniform2dv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform3dv(const std::string& name, double* value, int32_t count) {
        glUniform3dv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform4dv(const std::string& name, double* value, int32_t count) {
        glUniform4dv(getUniformLocation(name), count, value);
    }

    void Shader::setUniformMatrix3fv(const std::string& name, float* value, int32_t count, bool transpose) {
        glUniformMatrix3fv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix4fv(const std::string& name, float* value, int32_t count, bool transpose) {
        glUniformMatrix4fv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix2x3fv(const std::string& name, float* value, int32_t count, bool transpose) {
        glUniformMatrix2x3fv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix2x4fv(const std::string& name, float* value, int32_t count, bool transpose) {
        glUniformMatrix2x4fv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix3x2fv(const std::string& name, float* value, int32_t count, bool transpose) {
        glUniformMatrix3x2fv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix3x4fv(const std::string& name, float* value, int32_t count, bool transpose) {
        glUniformMatrix3x4fv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix4x2fv(const std::string& name, float* value, int32_t count, bool transpose) {
        glUniformMatrix4x2fv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix4x3fv(const std::string& name, float* value, int32_t count, bool transpose) {
        glUniformMatrix4x3fv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix3dv(const std::string& name, double* value, int32_t count, bool transpose) {
        glUniformMatrix3dv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix4dv(const std::string& name, double* value, int32_t count, bool transpose) {
        glUniformMatrix4dv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix2x3dv(const std::string& name, double* value, int32_t count, bool transpose) {
        glUniformMatrix2x3dv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix2x4dv(const std::string& name, double* value, int32_t count, bool transpose) {
        glUniformMatrix2x4dv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix3x2dv(const std::string& name, double* value, int32_t count, bool transpose) {
        glUniformMatrix3x2dv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix3x4dv(const std::string& name, double* value, int32_t count, bool transpose) {
        glUniformMatrix3x4dv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix4x2dv(const std::string& name, double* value, int32_t count, bool transpose) {
        glUniformMatrix4x2dv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix4x3dv(const std::string& name, double* value, int32_t count, bool transpose) {
        glUniformMatrix4x3dv(getUniformLocation(name), count, transpose, value);
    }
}