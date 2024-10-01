#pragma once

#include "../Core/Base.hpp"

#include <cstdint>
#include <string>

#include <glm/glm.hpp>

#define EVENT_CLASS_TYPE(type) \
    static EventType GetStaticType() { return EventType::type; } \
    virtual EventType GetEventType() const override { return GetStaticType(); } \
    virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
    virtual int32_t GetEventCategory() const override { return category; }

namespace Nickel2 {
    enum class EventType : uint32_t {
        None = 0, WindowClose, WindowMinimize, WindowMaximize, WindowResize, WindowMove, WindowFocus,
        KeyPress, KeyHold, KeyRelease, KeyType, MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll
    };

    enum EventCategory : uint32_t {
        None = 0, EventCategoryWindow = BIT(0), EventCategoryInput = BIT(1), EventCategoryKeyboard = BIT(2), EventCategoryMouse = BIT(3)
    };

    class Event {
        public:
            virtual ~Event() {}

            virtual const char* GetName() const = 0;
            virtual EventType GetEventType() const = 0;
            virtual int32_t GetEventCategory() const = 0;

            virtual std::string ToString() const { return GetName(); }
            inline bool IsInCategory(EventCategory category) { return GetEventCategory() & category; }
    };
}