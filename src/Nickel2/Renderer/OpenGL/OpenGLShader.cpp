#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/OpenGL/OpenGLShader.hpp>

namespace Nickel2 {
    namespace Utils {
        static uint32_t GetShaderStage(ShaderStage stage) {
            switch (stage) {
                case ShaderStage::Vertex: return GL_VERTEX_SHADER;
                case ShaderStage::Fragment: return GL_FRAGMENT_SHADER;
                case ShaderStage::Compute: return GL_COMPUTE_SHADER;
                case ShaderStage::Geometry: return GL_GEOMETRY_SHADER;
                case ShaderStage::TessellationControl: return GL_TESS_CONTROL_SHADER;
                case ShaderStage::TessellationEvaluation: return GL_TESS_EVALUATION_SHADER;
                default: return GL_NONE;
            }
        }

        static std::string GetShaderStageString(ShaderStage stage) {
            switch (stage) {
                case ShaderStage::Vertex: return "vertex";
                case ShaderStage::Fragment: return "fragment";
                case ShaderStage::Compute: return "compute";
                case ShaderStage::Geometry: return "geometry";
                case ShaderStage::TessellationControl: return "tessellation control";
                case ShaderStage::TessellationEvaluation: return "tessellation evaluation";
                default: return "unknown";
            }
        }
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::unordered_map<ShaderStage, std::string>& sources) : name(name) {
        this->CreateProgram(sources);
    }

    OpenGLShader::~OpenGLShader() {
        if (this->id != 0)
            this->DestroyProgram();
    }

    void OpenGLShader::CreateProgram(const std::unordered_map<ShaderStage, std::string>& sources) {
        if (this->id != 0)
            this->DestroyProgram();
            
        uint32_t program = glCreateProgram();
        std::vector<uint32_t> shaders;

        for (auto&& [stage, source] : sources) {
            uint32_t id = glCreateShader(Utils::GetShaderStage(stage));
            const char* sources[] = { source.c_str() };
            glShaderSource(id, 1, sources, nullptr);
            glCompileShader(id);

            int32_t status;
            glGetShaderiv(id, GL_COMPILE_STATUS, &status);

            if (!status) {
                int32_t length;
                glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
                std::vector<char> buffer(length);
                glGetShaderInfoLog(id, length, nullptr, buffer.data());
                std::string string(buffer.begin(), buffer.end());
                Logger::Log(Logger::Level::Error, "Shader", ("Failed to compile the " + Utils::GetShaderStageString(stage) + "shader: \n" + string).c_str());
                glDeleteShader(id);
            } else {
                glAttachShader(program, id);
                shaders.push_back(id);
            }
        }
        
        glLinkProgram(program);

        for (uint32_t shader : shaders)
            glDetachShader(program, shader);

        for (uint32_t shader : shaders)
            glDeleteShader(shader);

        int32_t isLinked;
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);

        if (!isLinked) {
            int32_t maxLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<char> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
            Logger::Log(Logger::Level::Error, "Shader", ("Failed to link the program: \n" + std::string(infoLog.data())).c_str());
            glDeleteProgram(program);
        } else {
            this->id = program;
        }
    }

    void OpenGLShader::DestroyProgram() {
        glDeleteProgram(this->id);
        this->id = 0;
    }

    int32_t OpenGLShader::GetUniformLocation(const std::string& name) {
        int32_t location = glGetUniformLocation(id, name.c_str());

        /* if (location == -1)
            Logger::Log(Logger::Level::Warn, "Shader", ("Uniform \"" + name + "\" not found in shader \"" + this->name + "\".").c_str()); */

        return location;
    }

    void OpenGLShader::Bind() const {
        glUseProgram(id);
    }

    void OpenGLShader::Unbind() const {
        glUseProgram(0);
    }

    void OpenGLShader::SetIntArray(const std::string& name, const int32_t* values, uint32_t count) {
        glUniform1iv(GetUniformLocation(name), count, values);
    }

    void OpenGLShader::SetIntArray2(const std::string& name, const glm::ivec2* values, uint32_t count) {
        glUniform2iv(GetUniformLocation(name), count, (int32_t*) values);
    }

    void OpenGLShader::SetIntArray3(const std::string& name, const glm::ivec3* values, uint32_t count) {
        glUniform3iv(GetUniformLocation(name), count, (int32_t*) values);
    }

    void OpenGLShader::SetIntArray4(const std::string& name, const glm::ivec4* values, uint32_t count) {
        glUniform4iv(GetUniformLocation(name), count, (int32_t*) values);
    }

    void OpenGLShader::SetInt(const std::string& name, int32_t value) {
        glUniform1i(GetUniformLocation(name), value);
    }

    void OpenGLShader::SetInt2(const std::string& name, const glm::ivec2& value) {
        glUniform2i(GetUniformLocation(name), value.x, value.y);
    }

    void OpenGLShader::SetInt3(const std::string& name, const glm::ivec3& value) {
        glUniform3i(GetUniformLocation(name), value.x, value.y, value.z);
    }

    void OpenGLShader::SetInt4(const std::string& name, const glm::ivec4& value) {
        glUniform4i(GetUniformLocation(name), value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::SetFloatArray(const std::string& name, const float* values, uint32_t count) {
        glUniform1fv(GetUniformLocation(name), count, values);
    }

    void OpenGLShader::SetFloatArray2(const std::string& name, const glm::vec2* values, uint32_t count) {
        glUniform2fv(GetUniformLocation(name), count, (float*) values);
    }

    void OpenGLShader::SetFloatArray3(const std::string& name, const glm::vec3* values, uint32_t count) {
        glUniform3fv(GetUniformLocation(name), count, (float*) values);
    }

    void OpenGLShader::SetFloatArray4(const std::string& name, const glm::vec4* values, uint32_t count) {
        glUniform4fv(GetUniformLocation(name), count, (float*) values);
    }

    void OpenGLShader::SetFloat(const std::string& name, float value) {
        glUniform1f(GetUniformLocation(name), value);
    }

    void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value) {
        glUniform2f(GetUniformLocation(name), value.x, value.y);
    }

    void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value) {
        glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
    }

    void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value) {
        glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& value) {
        glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value) {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    void OpenGLShader::SetMatArray3(const std::string& name, const glm::mat3* values, uint32_t count) {
        glUniformMatrix3fv(GetUniformLocation(name), count, GL_FALSE, (float*) values);
    }

    void OpenGLShader::SetMatArray4(const std::string& name, const glm::mat4* values, uint32_t count) {
        glUniformMatrix4fv(GetUniformLocation(name), count, GL_FALSE, (float*) values);
    }
}