#pragma once

#include <map>
#include <string>
#include "ERROR.hpp"

using namespace std;

string str_replace(const map<string, string>& v, const string& s) {
    string result = s;
    for (const auto& pair : v) {
        if (pair.first == pair.second) continue;
        if (pair.first.empty()) throw ERROR("Cannot replace from an empty string");
        size_t pos = 0;
        while ((pos = result.find(pair.first, pos)) != string::npos) {
            result.replace(pos, pair.first.length(), pair.second);
            if (!pair.second.empty()) pos += pair.second.length();
        }
    }
    return result;
}

string str_replace(const string& from, const string& to, const string& subject) {
    return str_replace({{from, to}}, subject);
}
