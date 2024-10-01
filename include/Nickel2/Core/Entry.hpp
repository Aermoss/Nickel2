#pragma once

#include "Application.hpp"

extern Nickel2::Application* Nickel2::CreateApplication(int32_t argc, const uint8_t* argv[]);

namespace Nickel2 {
    int32_t Main(int32_t argc, const uint8_t* argv[]) {
        Initialize();
        Application* app = CreateApplication(argc, argv);
        NK_CORE_ASSERT(app, "Couldn't create application.");
        app->Initialize();
        app->Terminate();
        delete app;
        Terminate();
        return 0;
    }
}

#if defined(NK_PLATFORM_WINDOWS)
    int32_t APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
        return Nickel2::Main(__argc, (const uint8_t**) __argv);
    }
#endif