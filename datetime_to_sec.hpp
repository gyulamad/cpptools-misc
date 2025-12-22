#pragma once

#include "datetime_to_ms.hpp"

using namespace std;

time_sec datetime_to_sec(const string& date) {
    return (time_sec)(datetime_to_ms(date) / second_ms);
}
