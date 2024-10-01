#pragma once

#include "Base.hpp"
#include "Logger.hpp"

#ifdef NK_ENABLE_ASSERTS
    #define NK_CORE_ASSERT_NO_CONDITION(...)
    #define NK_CORE_ASSERT_WITHOUT_MSG(condition) if (!(condition)) { NK_CORE_FATAL_ERROR_TAG((std::string(__FILE__) + ":" + std::to_string(__LINE__)).c_str(), "Assertion failed."); NK_DEBUGBREAK(); }
    #define NK_CORE_ASSERT_WITH_MSG(condition, message) if (!(condition)) { NK_CORE_FATAL_ERROR_TAG((std::string(__FILE__) + ":" + std::to_string(__LINE__)).c_str(), ("Assertion failed: " + std::string(message)).c_str()); NK_DEBUGBREAK(); }
    
    #define NK_OVERLOAD_CORE_ASSERT_MACRO(_0, _1, _2, OVERLOAD, ...) OVERLOAD
    #define NK_CORE_ASSERT(...) NK_OVERLOAD_CORE_ASSERT_MACRO(_0, ##__VA_ARGS__, NK_CORE_ASSERT_WITH_MSG, NK_CORE_ASSERT_WITHOUT_MSG, NK_CORE_ASSERT_NO_CONDITION)(__VA_ARGS__)
#else
    #define NK_CORE_ASSERT(...)
#endif