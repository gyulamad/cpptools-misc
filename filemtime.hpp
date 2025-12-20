#pragma once

#include "datetime_defs.hpp"
#include <string>
#include "filemtime_sec.hpp"

using namespace std;

time_sec filemtime(const string& filename) {
    return filemtime_sec(filename);
}
