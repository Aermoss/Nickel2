#pragma once

#include <Nickel2/Audio/AudioListener.hpp>

#include <al/al.h>
#include <al/alc.h>
#include <al/alut.h>

namespace Nickel2 {
    class OpenALListener : public AudioListener {
        private:
            virtual void Update(float deltaTime) override;

        public:
            OpenALListener(Entity* entity, DistanceModel distanceModel);
            ~OpenALListener();

            virtual void SetDistanceModel(DistanceModel distanceModel) override;
            virtual DistanceModel GetDistanceModel() override;
    };
}