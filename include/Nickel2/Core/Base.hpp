#pragma once

#include <memory>

#include "Version.hpp"
#include "Platform.hpp"

#ifdef NK_DEBUG
    #if defined(NK_PLATFORM_WINDOWS)
        #define NK_DEBUGBREAK() __debugbreak()
    #elif defined(NK_PLATFORM_LINUX)
        #include <signal.h>
        #define NK_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "platform doesn't support debugbreak."
    #endif
    #define NK_ENABLE_ASSERTS
#else
    #define NK_DEBUGBREAK()
#endif

#define NK_EXPAND_MACRO(x) x
#define NK_STRINGIFY_MACRO(x) #x

#define BIT(x) (1u << x)

namespace Nickel2 {
    void Initialize();
    void Terminate();

    template<typename T> using Scope = std::unique_ptr<T>;
    template<typename T, typename ... _args>
    constexpr Scope<T> createScope(_args&& ... args) {
        return std::make_unique<T>(std::forward<_args>(args)...);
    }

    template<typename T> using Ref = std::shared_ptr<T>;
    template<typename T, typename ... _args>
    constexpr Ref<T> createRef(_args&& ... args) {
        return std::make_shared<T>(std::forward<_args>(args)...);
    }
}

#include "Assert.hpp"