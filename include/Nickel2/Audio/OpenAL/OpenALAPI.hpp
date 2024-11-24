#pragma once

#include <Nickel2/Audio/AudioAPI.hpp>

namespace Nickel2 {
    class OpenALAPI : public AudioAPI {
        private:
            ALCcontext* context;

        public:
            OpenALAPI();
            ~OpenALAPI();

            virtual void Initialize() override;
            virtual void Terminate() override;
    };
}