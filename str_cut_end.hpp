#pragma once

#include <string>

using namespace std;

string str_cut_end(const string& s, size_t maxch = 300, const string& append = "...") {
    // Check if the string is already shorter than or equal to the limit
    if (s.length() <= maxch) return s;

    // Handle the case where maxch is smaller than or equal to the length of append
    if (maxch <= append.length()) return append;

    // Truncate the string and append the suffix
    return s.substr(0, maxch - append.length()) + append;
}

