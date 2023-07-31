#include "shader.hpp"

namespace nickel2 {
    uint32_t Shader::compileShader(uint32_t type, const char* source) {
        Context* context = getContext();
        uint32_t shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        int32_t status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        std::string name;
        if (type == GL_VERTEX_SHADER) name = "vertex";
        if (type == GL_FRAGMENT_SHADER) name = "fragment";
        if (type == GL_GEOMETRY_SHADER) name = "geometry";

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

    uint32_t Shader::createProgram(std::string vertexSource, std::string fragmentSource, std::string geometrySource) {
        Context* context = getContext();
        uint32_t vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource.c_str());
        uint32_t fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource.c_str());
        uint32_t geometryShader = 0;

        if (!geometrySource.empty())
            geometryShader = compileShader(GL_GEOMETRY_SHADER, geometrySource.c_str());

        uint32_t program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        if (geometryShader)
            glAttachShader(program, geometryShader);
        
        glLinkProgram(program);
        glDetachShader(program, vertexShader);
        glDeleteShader(vertexShader);
        glDetachShader(program, fragmentShader);
        glDeleteShader(fragmentShader);

        if (geometryShader) {
            glDetachShader(program, geometryShader);
            glDeleteShader(geometryShader);
        }

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

    Shader::Shader(std::string vertexSource, std::string fragmentSource, std::string geometrySource) {
        id = createProgram(vertexSource, fragmentSource, geometrySource);
    }

    Shader::~Shader() {

    }

    void Shader::use() {
        glUseProgram(id);
    }

    void Shader::unuse() {
        glUseProgram(0);
    }

    void Shader::reload(std::string vertexSource, std::string fragmentSource, std::string geometrySource) {
        glDeleteProgram(id);
        id = createProgram(vertexSource, fragmentSource, geometrySource);
    }

    void Shader::destroy() {
        glDeleteProgram(id);
    }

    int32_t Shader::getUniformLocation(const char* name) {
        return glGetUniformLocation(id, name);
    }

    void Shader::setUniform1i(const char* name, int32_t x) {
        glUniform1i(getUniformLocation(name), x);
    }

    void Shader::setUniform1ui(const char* name, uint32_t x) {
        glUniform1ui(getUniformLocation(name), x);
    }

    void Shader::setUniform2i(const char* name, int32_t x, int32_t y) {
        glUniform2i(getUniformLocation(name), x, y);
    }

    void Shader::setUniform2ui(const char* name, uint32_t x, uint32_t y) {
        glUniform2ui(getUniformLocation(name), x, y);
    }

    void Shader::setUniform3i(const char* name, int32_t x, int32_t y, int32_t z) {
        glUniform3i(getUniformLocation(name), x, y, z);
    }

    void Shader::setUniform3ui(const char* name, uint32_t x, uint32_t y, uint32_t z) {
        glUniform3ui(getUniformLocation(name), x, y, z);
    }

    void Shader::setUniform4i(const char* name, int32_t x, int32_t y, int32_t z, int32_t w) {
        glUniform4i(getUniformLocation(name), x, y, z, w);
    }

    void Shader::setUniform4ui(const char* name, uint32_t x, uint32_t y, uint32_t z, uint32_t w) {
        glUniform4ui(getUniformLocation(name), x, y, z, w);
    }

    void Shader::setUniform1iv(const char* name, int32_t* value, int32_t count) {
        glUniform1iv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform1uiv(const char* name, uint32_t* value, int32_t count) {
        glUniform1uiv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform2iv(const char* name, int32_t* value, int32_t count) {
        glUniform2iv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform2uiv(const char* name, uint32_t* value, int32_t count) {
        glUniform2uiv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform3iv(const char* name, int32_t* value, int32_t count) {
        glUniform3iv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform3uiv(const char* name, uint32_t* value, int32_t count) {
        glUniform3uiv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform4iv(const char* name, int32_t* value, int32_t count) {
        glUniform4iv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform4uiv(const char* name, uint32_t* value, int32_t count) {
        glUniform4uiv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform1f(const char* name, float x) {
        glUniform1f(getUniformLocation(name), x);
    }

    void Shader::setUniform2f(const char* name, float x, float y) {
        glUniform2f(getUniformLocation(name), x, y);
    }

    void Shader::setUniform3f(const char* name, float x, float y, float z) {
        glUniform3f(getUniformLocation(name), x, y, z);
    }

    void Shader::setUniform4f(const char* name, float x, float y, float z, float w) {
        glUniform4f(getUniformLocation(name), x, y, z, w);
    }

    void Shader::setUniform1fv(const char* name, float* value, int32_t count) {
        glUniform1fv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform2fv(const char* name, float* value, int32_t count) {
        glUniform2fv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform3fv(const char* name, float* value, int32_t count) {
        glUniform3fv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform4fv(const char* name, float* value, int32_t count) {
        glUniform4fv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform1d(const char* name, double x) {
        glUniform1d(getUniformLocation(name), x);
    }

    void Shader::setUniform2d(const char* name, double x, double y) {
        glUniform2d(getUniformLocation(name), x, y);
    }

    void Shader::setUniform3d(const char* name, double x, double y, double z) {
        glUniform3d(getUniformLocation(name), x, y, z);
    }

    void Shader::setUniform4d(const char* name, double x, double y, double z, double w) {
        glUniform4d(getUniformLocation(name), x, y, z, w);
    }

    void Shader::setUniform1dv(const char* name, double* value, int32_t count) {
        glUniform1dv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform2dv(const char* name, double* value, int32_t count) {
        glUniform2dv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform3dv(const char* name, double* value, int32_t count) {
        glUniform3dv(getUniformLocation(name), count, value);
    }

    void Shader::setUniform4dv(const char* name, double* value, int32_t count) {
        glUniform4dv(getUniformLocation(name), count, value);
    }

    void Shader::setUniformMatrix3fv(const char* name, float* value, int32_t count, bool transpose) {
        glUniformMatrix3fv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix4fv(const char* name, float* value, int32_t count, bool transpose) {
        glUniformMatrix4fv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix2x3fv(const char* name, float* value, int32_t count, bool transpose) {
        glUniformMatrix2x3fv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix2x4fv(const char* name, float* value, int32_t count, bool transpose) {
        glUniformMatrix2x4fv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix3x2fv(const char* name, float* value, int32_t count, bool transpose) {
        glUniformMatrix3x2fv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix3x4fv(const char* name, float* value, int32_t count, bool transpose) {
        glUniformMatrix3x4fv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix4x2fv(const char* name, float* value, int32_t count, bool transpose) {
        glUniformMatrix4x2fv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix4x3fv(const char* name, float* value, int32_t count, bool transpose) {
        glUniformMatrix4x3fv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix3dv(const char* name, double* value, int32_t count, bool transpose) {
        glUniformMatrix3dv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix4dv(const char* name, double* value, int32_t count, bool transpose) {
        glUniformMatrix4dv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix2x3dv(const char* name, double* value, int32_t count, bool transpose) {
        glUniformMatrix2x3dv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix2x4dv(const char* name, double* value, int32_t count, bool transpose) {
        glUniformMatrix2x4dv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix3x2dv(const char* name, double* value, int32_t count, bool transpose) {
        glUniformMatrix3x2dv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix3x4dv(const char* name, double* value, int32_t count, bool transpose) {
        glUniformMatrix3x4dv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix4x2dv(const char* name, double* value, int32_t count, bool transpose) {
        glUniformMatrix4x2dv(getUniformLocation(name), count, transpose, value);
    }

    void Shader::setUniformMatrix4x3dv(const char* name, double* value, int32_t count, bool transpose) {
        glUniformMatrix4x3dv(getUniformLocation(name), count, transpose, value);
    }
}