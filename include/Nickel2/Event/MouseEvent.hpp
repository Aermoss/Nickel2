#pragma once

#include "Event.hpp"
#include "../Core/KeyCodes.hpp"

namespace Nickel2 {
    class MouseMoveEvent : public Event {
        public:
            MouseMoveEvent(float x, float y) : x(x), y(y) {}

            inline glm::vec2 GetPosition() const { return { x, y }; }

            EVENT_CLASS_TYPE(MouseMove)
            EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

        private:
            float x, y;
    };

    class MouseScrollEvent : public Event {
        public:
            MouseScrollEvent(float xOffset, float yOffset) : xOffset(xOffset), yOffset(yOffset) {}

            inline glm::vec2 GetOffset() const { return { xOffset, yOffset }; }

            EVENT_CLASS_TYPE(MouseScroll)
            EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
            
        private:
            float xOffset, yOffset;
    };

    class MouseButtonEvent : public Event {
        protected:
            MouseButtonEvent(MouseButton button) : button(button) {}
            
            MouseButton button;

        public:
            inline MouseButton GetMouseButton() const { return button; }

            EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)  
    };

    class MouseButtonPressEvent : public MouseButtonEvent {
        public:
            MouseButtonPressEvent(MouseButton button) : MouseButtonEvent(button) {}

            EVENT_CLASS_TYPE(MouseButtonPress)
    };

    class MouseButtonReleaseEvent : public MouseButtonEvent {
        public:
            MouseButtonReleaseEvent(MouseButton button) : MouseButtonEvent(button) {}

            EVENT_CLASS_TYPE(MouseButtonRelease)
    };
}