#pragma once

#include <vector>
#include <string>
#include "ERROR.hpp"

using namespace std;

vector<string> explode(const string& delimiter, const string& str) {
    if (delimiter.empty())
        throw ERROR("Delimeter can not be empty.");
    vector<string> result;
    size_t start = 0;
    size_t end = str.find(delimiter);

    // Split the string by the delimiter
    while (end != string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    // Add the last part of the string
    result.push_back(str.substr(start));

    return result;
}
