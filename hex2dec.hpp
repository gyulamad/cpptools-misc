#pragma once

#include <cstring>
#include <string>
#include "strtoupper.hpp"
#include "ERROR.hpp"

using namespace std;

inline int hex2dec(string hex) {
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
