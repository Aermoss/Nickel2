#pragma once

#include <memory>
#include <string>

namespace Nickel2 {
    enum class AudioAPIType {
        None = 0, OpenAL
    };

    class AudioAPI {
        private:
            inline static AudioAPIType api = AudioAPIType::OpenAL;

        public:
            virtual ~AudioAPI() = default;

            virtual void Initialize() = 0;
            virtual void Terminate() = 0;

            static AudioAPIType GetAPI() { return api; }
            static std::unique_ptr<AudioAPI> Create();
    };
}