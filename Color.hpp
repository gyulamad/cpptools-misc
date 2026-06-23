#pragma once

#include <string>
#include "hex.hpp"

using namespace std;

class Color {
public:
    int r = 0, g = 0, b = 0, a = 0;

    Color() {}
    Color(string hex, char esc = '\0') {
        if (hex.empty()) return;
        if (hex[0] == esc) return;
        if (hex[0] == '#') hex = hex.substr(1);
        // Valid sizes: 3(RGB), 6(RGBO), 8(RGBAO). Invalid: <3, ==4, ==5, >6.
        if (hex.size() != 3 && hex.size() != 6 && hex.size() != 8) throw ERROR("Invalid color hex code");
        // Handle short RGB form (size==3): expand each digit to a pair
        if (hex.size() == 3) {
            r = hex2uint(string("") + hex[0] + hex[0]);
            g = hex2uint(string("") + hex[1] + hex[1]);
            b = hex2uint(string("") + hex[2] + hex[2]);
            a = 0;
            return;
        }
        // Full form: size==6 (RGB) or size==8 (RGBA) — pad to 8 if needed
        if (hex.size() == 6) hex += "00";
        r = hex2uint(hex.substr(0, 2));
        g = hex2uint(hex.substr(2, 2));
        b = hex2uint(hex.substr(4, 2));
        a = hex2uint(hex.substr(6, 2));
    }
    Color(const char* hex): Color(string(hex)) {}
    Color(unsigned int value): Color(uint2hex(value)) {}
    Color(int r, int g, int b, int a = 0): r(r), g(g), b(b), a(a) {}
    Color& operator=(const Color& other) {
        r = other.r;
        g = other.g;
        b = other.b;
        a = other.a;
        return *this;
    }
    virtual ~Color() {}
};
