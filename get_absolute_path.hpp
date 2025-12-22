#pragma once

#include <filesystem>
#include <string>
#include "fix_path.hpp"
#include "ERROR.hpp"
#include "file_exists.hpp"
#include "get_cwd.hpp"

using namespace std;

namespace fs = filesystem;

string get_absolute_path(const string& fname, bool throws = true) {
    string absolute = fs::absolute(fs::path(fname)).string();
    if (throws && !file_exists(absolute))
        throw ERROR("File not found (absolute): " + F(F_FILE, absolute) + ", originated from: " + F(F_FILE, fname) + ", cwd: " + get_cwd());
    string fixed = fix_path(absolute);
    if (throws && !file_exists(fixed))
        throw ERROR("File not found (fixed): " + F(F_FILE, fixed) + ", originated from: " + F(F_FILE, fname) + ", cwd: " + get_cwd());
    return fixed;
}
 
