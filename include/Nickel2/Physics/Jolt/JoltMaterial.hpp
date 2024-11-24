#pragma once

#include <Nickel2/Scene/Component.hpp>

#include <Jolt/Physics/Collision/PhysicsMaterial.h>

namespace Nickel2 {
    class JoltMaterial : public JPH::PhysicsMaterial {
        private:
            float friction, restitution;

        public:
            JoltMaterial() = default;
            JoltMaterial(float friction, float restitution) : friction(friction), restitution(restitution) {}

            float GetFriction() const { return friction; }
            void SetFriction(float friction) { this->friction = friction; }

            float GetRestitution() const { return restitution; }
            void SetRestitution(float restitution) { this->restitution = restitution; }

            inline static JoltMaterial* FromColliderMaterial(const ColliderMaterial& colliderMaterial) {
                return new JoltMaterial(colliderMaterial.friction, colliderMaterial.restitution);
            }
    };
}