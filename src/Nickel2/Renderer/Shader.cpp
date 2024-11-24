#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/Shader.hpp>
#include <Nickel2/Renderer/RendererAPI.hpp>

#include <Nickel2/Renderer/OpenGL/OpenGLShader.hpp>

namespace Nickel2 {
    std::shared_ptr<Shader> Shader::Create(const std::string& name, const std::unordered_map<ShaderStage, std::string>& sources) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPIType::OpenGL: return std::make_shared<OpenGLShader>(name, sources);
            default: return nullptr;
        }
    }

    const std::shared_ptr<Shader>& ShaderLibrary::Load(const std::string& name, const std::unordered_map<ShaderStage, std::string>& paths) {
        // NK_CORE_ASSERT(!this->Exists(name), ("Shader \"" + name + "\" already exists in the library.").c_str());
        std::unordered_map<ShaderStage, std::string> sources;
        shaderPaths[name] = paths;

        for (auto&& [stage, path] : paths)
            sources[stage] = Utils::ReadFile(path.c_str());

        this->Set(name, Shader::Create(name, sources));
        return this->Get(name);
    }

    void ShaderLibrary::Free(const std::shared_ptr<Shader>& shader) {
        return this->Free(shader->GetName());
    }

    void ShaderLibrary::Free(const std::string& name) {
        // NK_CORE_ASSERT(this->Exists(name), ("Shader \"" + name + "\" doesn't exist in the library.").c_str());
        shaderPaths.erase(name);
        this->Remove(name);
    }

    void ShaderLibrary::Add(const std::shared_ptr<Shader>& shader) {
        NK_CORE_ASSERT(!this->Exists(shader->GetName()), ("Shader \"" + shader->GetName() + "\" already exists in the library.").c_str());
        this->Set(shader->GetName(), shader);
    }

    void ShaderLibrary::Remove(const std::shared_ptr<Shader>& shader) {
        this->Remove(shader->GetName());
    }

    void ShaderLibrary::Remove(const std::string& name) {
        NK_CORE_ASSERT(this->Exists(name), ("Shader \"" + name + "\" doesn't exist in the library.").c_str());
        shaders.erase(name);
    }

    void ShaderLibrary::Set(const std::string& name, const std::shared_ptr<Shader>& shader) {
        shaders[name] = shader;
    }

    const std::shared_ptr<Shader>& ShaderLibrary::Get(const std::string& name) const {
        NK_CORE_ASSERT(this->Exists(name), ("Shader \"" + name + "\" doesn't exist in the library.").c_str());
        return shaders.at(name);
    }

    bool ShaderLibrary::Exists(const std::string& name) const {
        return shaders.find(name) != shaders.end();
    }

    bool ShaderLibrary::Exists(const std::shared_ptr<Shader>& shader) const {
        return this->Exists(shader->GetName());
    }

    void ShaderLibrary::Reload(const std::string& name) {
        NK_CORE_ASSERT(this->Exists(name), ("Shader \"" + name + "\" doesn't exist in the library.").c_str());
        this->Load(name, shaderPaths[name]);
    }

    void ShaderLibrary::Reload(const std::shared_ptr<Shader>& shader) {
        this->Reload(shader->GetName());
    }

    void ShaderLibrary::Reload() {
        for (auto&& [name, shader] : shaders)
            this->Reload(name);
    }
}