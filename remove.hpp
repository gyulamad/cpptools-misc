#pragma once

#include <filesystem>
#include <string>
#include "str_cut_begin.hpp"
#include "ERROR.hpp"

using namespace std;

namespace fs = filesystem; // Alias for convenience

bool remove(const string& path, bool throws = true) {
    bool ok = fs::remove(path);
    if (!ok && throws) throw ERROR("Unable to remove: " + str_cut_begin(path));
    return ok;
}
