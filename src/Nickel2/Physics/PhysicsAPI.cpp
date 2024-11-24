#include <Nickel2/nkpch.hpp>
#include <Nickel2/Physics/PhysicsAPI.hpp>

#include <Nickel2/Physics/Jolt/JoltAPI.hpp>

namespace Nickel2 {
    std::unique_ptr<PhysicsAPI> PhysicsAPI::Create() {
        switch (PhysicsAPI::GetAPI()) {
            case PhysicsAPIType::Jolt: return std::make_unique<JoltAPI>();
            default: return nullptr;
        }
    }
}