#pragma once

#include <string>

using namespace std;

string trim(const string& str, const string& chrs = " \t\n\r\f\v") {
    // Find the first non-whitespace character from the beginning
    size_t start = str.find_first_not_of(chrs);
    
    // If there is no non-whitespace character, return an empty string
    if (start == string::npos) return "";

    // Find the first non-whitespace character from the end
    size_t end = str.find_last_not_of(chrs);

    // Return the substring from the first non-whitespace to the last non-whitespace character
    return str.substr(start, end - start + 1);
}

