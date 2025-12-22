#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "implode.hpp"

using namespace std;

template<typename T>
string array_dump(const vector<T>& vec, bool show = false) {
    vector<string> dump;
    for (const T& elem : vec) {
        stringstream ss;
        ss << elem;
        dump.push_back(ss.str());
    }

    string output = "vector(" + to_string(vec.size()) + "): (" + implode(", ", dump) + ")";
    if (show) cout << output << endl;
    return output;
}
    
