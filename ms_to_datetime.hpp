#pragma once

#include "get_time_ms.hpp"
#include <sstream>
#include <iomanip>

using namespace std;

string ms_to_datetime(time_ms ms = get_time_ms(), const char* fmt = "%Y-%m-%d %H:%M:%S", bool millis = true, bool local = false) {
    long sec = (signed)(ms / second_ms);
    long mil = (signed)(ms % second_ms);

    struct tm converted_time;
    if (local) localtime_r(&sec, &converted_time);
    else gmtime_r(&sec, &converted_time);

    ostringstream oss;
    oss << put_time(&converted_time, fmt);

    if (millis) oss << "." << setfill('0') << setw(3) << mil;

    return oss.str();
}

