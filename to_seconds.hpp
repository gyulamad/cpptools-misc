#pragma once

#include <string>
#include "to_milliseconds.hpp"

using namespace std;

double to_seconds(const string& period_str) {
    return to_milliseconds(period_str) / 1000.0;
}
