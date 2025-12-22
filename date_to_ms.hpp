#pragma once

#include <string>
#include "datetime_to_ms.hpp"

using namespace std;

time_t date_to_ms(const string& date) {
    return datetime_to_ms(date);
}
