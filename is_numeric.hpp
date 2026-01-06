#pragma once

#include <string>
#include <cctype>

using namespace std;

inline bool is_numeric(const string& str) {
    if (str.empty())
        return false;
    
    int i = 0;
    int len = str.length();
    
    // Handle optional leading sign
    if (str[i] == '+' || str[i] == '-')
        i++;
    
    // Must have at least one digit after sign (if present)
    if (i >= len)
        return false;
    
    bool has_digit = false;
    bool has_dot = false;
    bool has_exponent = false;
    
    while (i < len) {
        char c = str[i];
        
        if (isdigit(c)) {
            has_digit = true;
        }
        else if (c == '.') {
            // Only one decimal point allowed, and not after exponent
            if (has_dot || has_exponent)
                return false;
            has_dot = true;
        }
        else if (c == 'e' || c == 'E') {
            // Exponent requires at least one digit before it
            if (!has_digit || has_exponent)
                return false;
            has_exponent = true;
            has_digit = false; // Reset to check for digits after exponent
            
            // Check for optional sign after exponent
            if (i + 1 < len && (str[i + 1] == '+' || str[i + 1] == '-'))
                i++;
        }
        else {
            return false; // Invalid character
        }
        
        i++;
    }
    
    // Must have at least one digit
    return has_digit;
}
