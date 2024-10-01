#pragma once

namespace Nickel2 {
    struct Color {
        float r, g, b, a;

        Color(glm::vec3 vec) : r(vec.r), g(vec.g), b(vec.b), a(1.0f) {}
        Color(glm::vec4 vec) : r(vec.r), g(vec.g), b(vec.b), a(vec.a) {}

        Color(float r, float g, float b) : r(r), g(g), b(b), a(1.0f) {}
        Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

        Color(int32_t r, int32_t g, int32_t b) : r(r / 255.0f), g(g / 255.0f), b(b / 255.0f), a(1.0f) {}
        Color(int32_t r, int32_t g, int32_t b, int32_t a) : r(r / 255.0f), g(g / 255.0f), b(b / 255.0f), a(a / 255.0f) {}

        operator float*() { return &r; }
        operator const float*() const { return &r; }

        void FromVec3(const glm::vec3& vec) { r = vec.r, g = vec.g, b = vec.b; }
        void FromVec4(const glm::vec4& vec) { r = vec.r, g = vec.g, b = vec.b, a = vec.a; }

        glm::vec3 ToVec3() const { return {r, g, b}; }
        glm::vec4 ToVec4() const { return {r, g, b, a}; }

        void Clamp() { r = std::clamp(r, 0.0f, 1.0f), g = std::clamp(g, 0.0f, 1.0f), b = std::clamp(b, 0.0f, 1.0f), a = std::clamp(a, 0.0f, 1.0f); }
        void Invert() { r = 1.0f - r, g = 1.0f - g, b = 1.0f - b, a = 1.0f - a; }

        Color Clamped() const { return {std::clamp(r, 0.0f, 1.0f), std::clamp(g, 0.0f, 1.0f), std::clamp(b, 0.0f, 1.0f), std::clamp(a, 0.0f, 1.0f)}; }
        Color Inverted() const { return {1.0f - r, 1.0f - g, 1.0f - b, 1.0f - a}; }

        Color operator+(const Color& color) const { return {r + color.r, g + color.g, b + color.b, a + color.a}; }
        Color operator-(const Color& color) const { return {r - color.r, g - color.g, b - color.b, a - color.a}; }
        Color operator*(const Color& color) const { return {r * color.r, g * color.g, b * color.b, a * color.a}; }
        Color operator/(const Color& color) const { return {r / color.r, g / color.g, b / color.b, a / color.a}; }

        Color operator+(float scalar) const { return {r + scalar, g + scalar, b + scalar, a + scalar}; }
        Color operator-(float scalar) const { return {r - scalar, g - scalar, b - scalar, a - scalar}; }
        Color operator*(float scalar) const { return {r * scalar, g * scalar, b * scalar, a * scalar}; }
        Color operator/(float scalar) const { return {r / scalar, g / scalar, b / scalar, a / scalar}; }

        Color& operator+=(const Color& color) { return *this = *this + color; }
        Color& operator-=(const Color& color) { return *this = *this - color; }
        Color& operator*=(const Color& color) { return *this = *this * color; }
        Color& operator/=(const Color& color) { return *this = *this / color; }

        Color& operator+=(float scalar) { return *this = *this + scalar; }
        Color& operator-=(float scalar) { return *this = *this - scalar; }
        Color& operator*=(float scalar) { return *this = *this * scalar; }
        Color& operator/=(float scalar) { return *this = *this / scalar; }

        bool operator==(const Color& color) const { return r == color.r && g == color.g && b == color.b && a == color.a; }
        bool operator!=(const Color& color) const { return r != color.r || g != color.g || b != color.b || a != color.a; }

        static Color Lerp(const Color& a, const Color& b, float t) { return a + (b - a) * t; }
    };
}