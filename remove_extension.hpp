#pragma once

#include <string>
#include <vector>
#include "explode.hpp"
#include "implode.hpp"

using namespace std;

string remove_extension(const string& fname) {
    vector<string> splits_path = explode("/", fname);
    vector<string> splits_last = explode(".", splits_path[splits_path.size() - 1]);
    if (splits_last.size() > 1) splits_last.pop_back(); // remove last element but only if it had extension
    splits_path[splits_path.size() - 1] = implode(".", splits_last);
    return implode("/", splits_path);
}
