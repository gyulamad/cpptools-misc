#pragma once

#include <string>

using namespace std;

// PHP like function to convert string to lower case
string strtolower(const string& s) {
    string result = s; // Create a copy of the input string
    for (char& c : result) // Iterate over each character
        c = tolower(static_cast<unsigned char>(c)); // Convert to lower case
    return result;        
}
    
