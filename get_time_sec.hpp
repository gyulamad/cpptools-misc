#pragma once

#include "default_get_time_sec.hpp"

using namespace std;

// Declaration in the header file (e.g., datetime.hpp)
// extern time_t (*get_time_sec)();

inline time_sec (*get_time_sec)() = default_get_time_sec;
