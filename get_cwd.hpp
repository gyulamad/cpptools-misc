#pragma once

#include <string>
#include <filesystem>
#include "fix_path.hpp"

using namespace std;

namespace fs = filesystem;

// Helper to get the expected base path
string get_cwd() {
    return fix_path(fs::current_path().string());
}
