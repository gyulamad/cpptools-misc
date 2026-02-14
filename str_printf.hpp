#pragma once

#include <string>
#include <cstdarg>

using namespace std;

inline string str_printf(const char* fmt, ...) {
    // Handle empty format string
    if (!fmt || fmt[0] == '\0') {
        return "";
    }
    
    // Get required length
    va_list args1;
    va_start(args1, fmt);
    int size = vsnprintf(nullptr, 0, fmt, args1);
    va_end(args1);
    
    if (size < 0) {
        return "";
    }
    
    // Allocate buffer (size doesn't include null terminator)
    string s(size, '\0');
    
    // Write formatted string
    va_list args2;
    va_start(args2, fmt);
    vsnprintf(s.data(), size + 1, fmt, args2);
    va_end(args2);
    
    return s;
}
