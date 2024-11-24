#include <Nickel2/nkpch.hpp>
#include <Nickel2/Physics/Jolt/JoltAPI.hpp>
#include <Nickel2/Physics/Jolt/JoltCookingFactory.hpp>
#include <Nickel2/Physics/Jolt/JoltScene.hpp>

#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

namespace Nickel2 {
    struct JoltData {
        JPH::TempAllocator* temporariesAllocator;
        std::shared_ptr<JoltCookingFactory> cookingFactory = nullptr;
        std::unique_ptr<JPH::JobSystemThreadPool> jobThreadPool;
        std::string lastErrorMessage = "";

    };

    static JoltData* joltData = nullptr;

    static void JoltTraceCallback(const char* format, ...) {
        va_list list;
        va_start(list, format);
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), format, list);
        if (joltData) joltData->lastErrorMessage = buffer;
        std::string string = std::string(buffer);
        string = string[string.size() - 1] != '.' ? string + "." : string;
        Logger::Log(Logger::Level::Trace, "Physics", string.c_str());
    }

#ifdef JPH_ENABLE_ASSERTS
    static bool JoltAssertFailedCallback(const char* expression, const char* message, const char* file, uint32_t line) {
        std::string string = std::string(message);
        string = string[string.size() - 1] != '.' ? string + "." : string;
        Logger::Log(Logger::Level::Error, "Physics", string.c_str());
        return true;
    }
#endif

    JoltAPI::JoltAPI() {}
    JoltAPI::~JoltAPI() {}

    void JoltAPI::Initialize() {
        NK_CORE_ASSERT(!joltData, "Can't initialize Jolt multiple times.");
        JPH::RegisterDefaultAllocator();
        JPH::Trace = JoltTraceCallback;

#ifdef JPH_ENABLE_ASSERTS
        JPH::AssertFailed = JoltAssertFailedCallback;
#endif

        JPH::Factory::sInstance = new JPH::Factory();
        JPH::RegisterTypes();

        joltData = new JoltData();
        joltData->temporariesAllocator = new JPH::TempAllocatorMalloc();
        // joltData->temporariesAllocator = new JPH::TempAllocatorImpl(300 * 1024 * 1024);
        joltData->jobThreadPool = std::make_unique<JPH::JobSystemThreadPool>(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);
        joltData->cookingFactory = std::make_shared<JoltCookingFactory>();
        joltData->cookingFactory->Initialize();
    }

    void JoltAPI::Terminate() {
        joltData->cookingFactory->Terminate();
        joltData->cookingFactory = nullptr;

        delete joltData->temporariesAllocator;
        delete joltData;

        joltData = nullptr;
        delete JPH::Factory::sInstance;
    }

    std::shared_ptr<PhysicsScene> JoltAPI::CreateScene(Scene* scene) const {
        return std::make_shared<JoltScene>(scene);
    }

    JPH::TempAllocator* JoltAPI::GetTempAllocator() const { return joltData->temporariesAllocator; }
    JPH::JobSystemThreadPool* JoltAPI::GetJobThreadPool() const { return joltData->jobThreadPool.get(); }

    std::shared_ptr<MeshCookingFactory> JoltAPI::GetMeshCookingFactory() const { return joltData->cookingFactory; }
    const std::string& JoltAPI::GetLastError() const { return joltData->lastErrorMessage; }
}