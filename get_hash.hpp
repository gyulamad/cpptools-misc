#pragma once

#include <string>

using namespace std;

inline string get_hash(const string& str) {
    return to_string(hash<string>{}(str));
}
