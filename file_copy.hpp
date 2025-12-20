#pragma once

#include <string>
#include "is_dir.hpp"
#include "fix_path.hpp"
#include "get_filename.hpp"
#include "ERROR.hpp"
#include "file_exists.hpp"
#include "unlink.hpp"
#include "EWHAT.hpp"

using namespace std;

bool file_copy(const string& from, string to, bool override, bool throws = true) {
    if (is_dir(to)) to = fix_path(to + "/" + get_filename(from));

    // Check if source file exists and is readable
    error_code ec;
    if (!filesystem::exists(from, ec) || ec) {
        if (throws)
            throw ERROR("Source file is not exists or readable: " + F(F_FILE, from));
        return false;
    }

    // Check if destination exists and override is false
    if (!override && filesystem::exists(to, ec) && !ec) {
        if (throws)
            throw ERROR("Destination exists but override is disabled: " + F(F_FILE, to));
        return false;
    }

    try {
        // overwrite existing if override is true
        if (override && file_exists(to)) unlink(to);        
        return filesystem::copy_file(from, to);
    } catch (exception& e) {
        if (throws)
            throw ERROR("File copy failed" + EWHAT);
        return false;
    }
}
