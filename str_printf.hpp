#pragma once

#include <string>
#include "ERROR.hpp"

using namespace std;

string str_printf(const string& /*fmt*/, auto... /*args*/) {
    ERROR("Needs to be implemented!");
    return "";
    // int size = snprintf(nullptr, 0, fmt.c_str(), args...);  // Get required length
    // string s(size, '\0');                           // Allocate (excluding null terminator)
    // snprintf(s.data(), size + 1, fmt.c_str(), args...);     // Write into it
    // return s;
}

