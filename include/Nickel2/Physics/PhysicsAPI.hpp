#pragma once

#include <Nickel2/Physics/PhysicsScene.hpp>
#include <Nickel2/Physics/MeshCookingFactory.hpp>

#include <string>

namespace Nickel2 {
    enum class PhysicsAPIType {
        None = 0, Jolt
    };

    class PhysicsAPI {
        private:
            inline static PhysicsAPIType api = PhysicsAPIType::Jolt;

        public:
            virtual ~PhysicsAPI() = default;

            virtual void Initialize() = 0;
            virtual void Terminate() = 0;
            
            virtual std::shared_ptr<PhysicsScene> CreateScene(Scene* scene) const = 0;

            virtual const std::string& GetLastError() const = 0;
            virtual std::shared_ptr<MeshCookingFactory> GetMeshCookingFactory() const = 0;

            static PhysicsAPIType GetAPI() { return api; }
            static std::unique_ptr<PhysicsAPI> Create();
    };
}