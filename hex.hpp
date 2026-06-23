#pragma once

#include <cstring>
#include <string>
#include "strtoupper.hpp"
#include "ERROR.hpp"

using namespace std;

inline unsigned int hex2uint(string hex) {
    if (hex.empty()) throw ERROR("Empty hex value");
    static const char* digits = "0123456789ABCDEF";
    hex = strtoupper(hex);
    int result = 0;
    for (char chr : hex) {
        const char* pos = strchr(digits, chr);
        if (!pos) throw ERROR("Invalid hex character: " + string(1, chr));
        result = result * 16 + (int)(pos - digits);
    }
    return result;
}

inline string uint2hex(unsigned int value) {
    unsigned int uval = (unsigned int)value;
    static const char* digits = "0123456789ABCDEF";
    if (uval == 0) return "0";
    string result;
    while (value > 0) {
        result = digits[value % 16] + result;
        value /= 16;
    }
    return result;
}