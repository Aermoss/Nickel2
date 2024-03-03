#pragma once

#include <memory>

#include "Platform.hpp"

#ifdef NICKEL2_DEBUG
    #if defined(NICKEL2_PLATFORM_WINDOWS)
        #define NICKEL2_DEBUGBREAK() __debugbreak()
    #elif defined(NICKEL2_PLATFORM_LINUX)
        #include <signal.h>
        #define NICKEL2_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "platform doesn't support debugbreak."
    #endif
    #define NICKEL2_ENABLE_ASSERTS
#else
    #define NICKEL2_DEBUGBREAK()
#endif

#define NICKEL2_EXPAND_MACRO(x) x
#define NICKEL2_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define NICKEL2_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Nickel2 {
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