#pragma once

#include "default_get_time_ms.hpp"

using namespace std;

// Declaration in the header file (e.g., datetime.hpp)
extern time_ms (*get_time_ms)();

time_ms (*get_time_ms)() = default_get_time_ms;

