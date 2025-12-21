#pragma once

#include <string>

using namespace std;

bool str_starts_with(const string& str, const string& prefix) {
    return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}

