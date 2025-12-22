#pragma once

#include <string>

using namespace std;

string str_cut_begin(const string& s, size_t maxch = 300, const string& prepend = "...") {
    // Check if the string is already shorter than or equal to the limit
    if (s.length() <= maxch) return s;

    // Handle the case where maxch is smaller than the length of prepend
    if (maxch <= prepend.length()) return prepend;
    
    // Truncate the string from the beginning and prepend the prefix
    return prepend + s.substr(s.length() - (maxch - prepend.length()));
}
