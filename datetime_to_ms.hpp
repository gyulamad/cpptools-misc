#pragma once

#include "is_valid_datetime.hpp"
#include "datetime_defs.hpp"
#include "EMPTY_OR.hpp"
#include "ERROR.hpp"

using namespace std;

time_ms datetime_to_ms(const string& datetime) {
    if (datetime.empty() || !is_valid_datetime(datetime)) 
        throw ERROR("Invalid datetime format: " + EMPTY_OR(datetime));

    struct tm time_info = {};
    int milliseconds = 0;
    
    size_t size = datetime.size();
    time_info.tm_year = (size > 3 ? stoi(datetime.substr(0, 4)) : 1970) - 1900;
    time_info.tm_mon = (size > 6 ? stoi(datetime.substr(5, 2)) : 1) - 1;
    time_info.tm_mday = size > 9 ? stoi(datetime.substr(8, 2)) : 1;
    time_info.tm_hour = size > 12 ? stoi(datetime.substr(11, 2)) : 0;
    time_info.tm_min = size > 15 ? stoi(datetime.substr(14, 2)) : 0;
    time_info.tm_sec = size > 18 ? stoi(datetime.substr(17, 2)) : 0;
    milliseconds = size > 22 ? stoi(datetime.substr(20, 3)) : 0;

    // Convert the struct tm to milliseconds
    time_ms seconds = mktime(&time_info);
    return seconds * second_ms + (unsigned)milliseconds;
}

