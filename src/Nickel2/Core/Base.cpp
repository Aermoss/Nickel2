#include <Nickel2/nkpch.hpp>
#include <Nickel2/Core/Base.hpp>

namespace Nickel2 {
    void Initialize() {
        Logger::Initialize();
        Logger::Log(Logger::Level::Info, "Core", ("Engine initialized successfully, version: \"" + std::string(NK_VERSION_STRING) + "\".").c_str());
    }

    void Terminate() {
        Logger::Log(Logger::Level::Info, "Core", "Engine terminated successfully.");
        Logger::Terminate();
    }
}