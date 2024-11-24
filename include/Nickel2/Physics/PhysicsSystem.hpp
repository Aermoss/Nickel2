#pragma once

#include "PhysicsSettings.hpp"
#include "PhysicsAPI.hpp"

namespace Nickel2 {
    class Scene;

    class PhysicsSystem {
        private:
            static std::unique_ptr<PhysicsAPI> api;
            inline static PhysicsSettings settings;

        public:
            static void Initialize() { api->Initialize(); }
            static void Terminate() { api->Terminate(); }

            static std::shared_ptr<PhysicsScene> CreateScene(Scene* scene);

            static PhysicsSettings& GetSettings() { return settings; }
            static const std::string& GetLastError() { return api->GetLastError(); }
            static std::shared_ptr<MeshCookingFactory> GetMeshCookingFactory() { return api->GetMeshCookingFactory(); }

            static PhysicsAPI* GetAPI() { return api.get(); }
    };
}