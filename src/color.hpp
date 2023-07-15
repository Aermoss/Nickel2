#pragma once

namespace nickel2 {
    struct Color {
        float r, g, b, a;

        Color(float r, float g, float b) : r(r), g(g), b(b), a(255) {}
        Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
    };
}