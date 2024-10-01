#pragma once

#include "Event.hpp"

namespace Nickel2 {
    class WindowCloseEvent : public Event {
        public:
            WindowCloseEvent() {}

            EVENT_CLASS_TYPE(WindowClose)
            EVENT_CLASS_CATEGORY(EventCategoryWindow)
    };

    class WindowMinimizeEvent : public Event {
        private:
            bool minimized = false;

        public:
            WindowMinimizeEvent(bool minimized) : minimized(minimized) {}

            bool IsMinimized() const { return minimized; }

            EVENT_CLASS_TYPE(WindowMinimize)
            EVENT_CLASS_CATEGORY(EventCategoryWindow)
    };

    class WindowMaximizeEvent : public Event {
        private:
            bool maximized = false;

        public:
            WindowMaximizeEvent(bool maximized) : maximized(maximized) {}

            bool IsMaximized() const { return maximized; }

            EVENT_CLASS_TYPE(WindowMaximize)
            EVENT_CLASS_CATEGORY(EventCategoryWindow)
    };

    class WindowResizeEvent : public Event {
        private:
            uint32_t width, height;

        public:
            WindowResizeEvent(uint32_t width, uint32_t height) : width(width), height(height) {}

            inline glm::vec2 GetSize() const { return { (float) width, (float) height }; }

            EVENT_CLASS_TYPE(WindowResize)
            EVENT_CLASS_CATEGORY(EventCategoryWindow)
    };

    class WindowMoveEvent : public Event {
        private:
            uint32_t x, y;

        public:
            WindowMoveEvent(uint32_t x, uint32_t y) : x(x), y(y) {}

            inline glm::vec2 GetPosition() const { return { (float) x, (float) y }; }

            EVENT_CLASS_TYPE(WindowMove)
            EVENT_CLASS_CATEGORY(EventCategoryWindow)
    };

    class WindowFocusEvent : public Event {
        private:
            bool focused = false;

        public:
            WindowFocusEvent(bool focused) : focused(focused) {}

            inline uint32_t IsFocused() const { return focused; }

            EVENT_CLASS_TYPE(WindowFocus)
            EVENT_CLASS_CATEGORY(EventCategoryWindow)
    };
}