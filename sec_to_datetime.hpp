#pragma once

#include "datetime_defs.hpp"
#include "ms_to_datetime.hpp"

using namespace std;

string sec_to_datetime(time_sec sec, const char *fmt = "%Y-%m-%d %H:%M:%S", bool local = false) {
    return ms_to_datetime((time_ms)sec * second_ms, fmt, false, local);
}
 
