#pragma once

#include <filesystem> // Requires C++17 or later
#include <string>

using namespace std;

namespace fs = filesystem; // Alias for convenience

// Function to check if a directory exists
bool is_dir(const string& path) {
    return fs::exists(path) && fs::is_directory(path);
}
