#pragma once

#include <filesystem>

using namespace std;

namespace fs = filesystem;

string get_filename_only(const string& fname) {
    return fs::path(fname).stem().string();
}
