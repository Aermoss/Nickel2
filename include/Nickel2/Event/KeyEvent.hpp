#pragma once

#include "Event.hpp"
#include "../Core/KeyCodes.hpp"

namespace Nickel2 {
    class KeyEvent : public Event {
        protected:
            KeyEvent(KeyCode keyCode) : keyCode(keyCode) {}

            KeyCode keyCode;
            
        public:
            inline KeyCode GetKeyCode() const { return keyCode; }

            EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    };

    class KeyPressEvent : public KeyEvent {
        public:
            KeyPressEvent(KeyCode keyCode) : KeyEvent(keyCode) {}

            EVENT_CLASS_TYPE(KeyPress)
    };

    class KeyHoldEvent : public KeyEvent {
        public:
            KeyHoldEvent(KeyCode keyCode) : KeyEvent(keyCode) {}

            EVENT_CLASS_TYPE(KeyHold)
    };

    class KeyReleaseEvent : public KeyEvent {
        public:
            KeyReleaseEvent(KeyCode keycode) : KeyEvent(keycode) {}

            EVENT_CLASS_TYPE(KeyRelease)
    };

    class KeyTypeEvent : public KeyEvent {
        public:
            KeyTypeEvent(KeyCode keycode) : KeyEvent(keycode) {}

            EVENT_CLASS_TYPE(KeyType)
    };
}