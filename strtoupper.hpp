#pragma once

#include <string>

using namespace std;

// PHP like function to convert string to upper case
string strtoupper(const string& s) {
    string result = s; // Create a copy of the input string
    for (char& c : result) // Iterate over each character
        c = toupper(static_cast<unsigned char>(c)); // Convert to upper case
    return result;
}

