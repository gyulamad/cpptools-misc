#pragma once

#include "in_array.hpp"
#include <algorithm>
#include "EMPTY_OR.hpp"
#include "ERROR.hpp"
#include <vector>

using namespace std;

template <typename T>
T parse(const string& str) {
    if constexpr (is_same_v<T, string>) {
        return str; // Return the string directly
    } else {
        static_assert(is_arithmetic<T>::value, "T must be an arithmetic type");
        stringstream ss(str);
        T num;
        
        // Handle special floating-point values for float and double
        if constexpr (is_floating_point_v<T>) {
            string lower = str;
            transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
            if (lower == "inf" || lower == "+inf") return numeric_limits<T>::infinity();
            if (lower == "-inf") return -numeric_limits<T>::infinity();
            if (lower == "nan") return numeric_limits<T>::quiet_NaN();
        }
        
        if (ss >> num)
            return num;
        
        throw ERROR("Invalid input string (not a number): " + EMPTY_OR(str));
    }
}

// Specialization for bool
template <>
bool parse<bool>(const string& str) {
    string lower = str;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);        
    if (in_array(lower, vector<string>({ "true", "on", "1", "yes"}))) return true;
    if (in_array(lower, vector<string>({ "false", "off", "0", "no"}))) return false;
    throw ERROR("Invalid input string (not a boolean): " + EMPTY_OR(str));
}

