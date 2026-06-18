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
        while (hex.size() <= 3) hex += "0";
        if (hex.size() == 4) {
            r = hex2uint(string("") + hex[0] + hex[0]);
            g = hex2uint(string("") + hex[1] + hex[1]);
            b = hex2uint(string("") + hex[2] + hex[2]);
            a = hex2uint(string("") + hex[3] + hex[3]);
            return;
        }
        if (hex.size() == 6) hex += "00";
        if (hex.size() == 8) {
            r = hex2uint(hex.substr(0, 2));
            g = hex2uint(hex.substr(2, 2));
            b = hex2uint(hex.substr(4, 2));
            a = hex2uint(hex.substr(6, 2));
            return;
        }
        throw ERROR("Invalid color hex code");
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
