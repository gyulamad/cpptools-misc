#pragma once

#include <filesystem>
#include <string>
#include "fix_path.hpp"

using namespace std;

namespace fs = filesystem;

string get_path(const string& pname) {
    if (pname.empty()) return "";
    string fixed = fix_path(fs::path(pname).parent_path().string());
    if (fixed.empty()) fixed = fix_path(fs::path(fs::absolute(pname)).parent_path().string());
    return fixed;
}

