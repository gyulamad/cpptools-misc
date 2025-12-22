#pragma once

#include <string>

using namespace std;

bool str_ends_with(const string& str, const string& prefix) {
    return str.size() >= prefix.size() && str.compare(str.size() - prefix.size(), prefix.size(), prefix) == 0;
}
