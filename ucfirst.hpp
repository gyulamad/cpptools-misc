#pragma once

#include <string>

using namespace std;

inline string ucfirst(const string& str) {
    if (str.empty()) {
        return str;
    }
    string result = str;
    result[0] = toupper(result[0]);
    return result;
}
