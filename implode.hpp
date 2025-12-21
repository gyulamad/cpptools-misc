#pragma once

#include <sstream>
#include <vector>
#include <string>

using namespace std;

string implode(const string& delimiter, const vector<string>& elements) {
    ostringstream oss;
    for (size_t i = 0; i < elements.size(); ++i) {
        if (i != 0) oss << delimiter;
        oss << elements[i];
    }
    return oss.str();
}

