#pragma once

#include <Nickel2/Physics/PhysicsAPI.hpp>
#include <Nickel2/Physics/PhysicsScene.hpp>

#include <Jolt/Jolt.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/Profiler.h>

namespace Nickel2 {
    class JoltAPI : public PhysicsAPI {
        public:
            JoltAPI();
            ~JoltAPI();

            virtual void Initialize() override;
            virtual void Terminate() override;

            JPH::TempAllocator* GetTempAllocator() const;
            JPH::JobSystemThreadPool* GetJobThreadPool() const;
            
            virtual std::shared_ptr<MeshCookingFactory> GetMeshCookingFactory() const override;
            virtual const std::string& GetLastError() const override;
            
            virtual std::shared_ptr<PhysicsScene> CreateScene(Scene* scene) const override;
    };
}