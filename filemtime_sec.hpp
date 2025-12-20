#pragma once

#include "datetime_defs.hpp"
#include <string>
#include <chrono>
#include "filemtime_duration.hpp"

using namespace std;

time_sec filemtime_sec(const string& filename) {
    return chrono::duration_cast<chrono::seconds>(filemtime_duration(filename)).count();
}
