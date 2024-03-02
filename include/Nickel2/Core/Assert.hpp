#pragma once

#include "Logger.hpp"
#include "Base.hpp"

#ifdef NICKEL2_ENABLE_ASSERTS
    #define NICKEL2_ASSERT(check, message) if (check) { NICKEL2_LOG(NICKEL2_ERROR, message); NICKEL2_DEBUGBREAK(); }
#else
    #define NICKEL2_ASSERT(...)
#endif