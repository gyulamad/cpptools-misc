#pragma once

#include <string>

using namespace std;

bool is_valid_datetime(const string& datetime) {
    if (datetime.empty()) return false;

    const string tpl = "0000-01-01 00:00:00.000";
    size_t len = datetime.length();
    if (len > tpl.length()) return false;

    for (size_t i = 0; i < len; ++i) {
        char expected = tpl[i];
        char actual = datetime[i];

        if (expected == '0') {
            if (!isdigit(actual)) return false;
        } else if (expected == '-') {
            if (actual != '-') return false;
        } else if (expected == ':') {
            if (actual != ':') return false;
        } else if (expected == '.') {
            if (actual != '.') return false;
        } else if (expected == ' ') {
            if (actual != ' ') return false;
        }
    }

    // Check if a '.' exists at position 19 (millisecond separator)
    if (len > 19 && datetime[19] == '.') {
        // If '.' is present, ensure there are exactly 3 digits after it
        if (len != 23 || 
            !isdigit(datetime[20]) || 
            !isdigit(datetime[21]) || 
            !isdigit(datetime[22])) {
            return false;
        }
    }

    // Parse components (unchanged)
    int year = 0, month = 1, day = 1, hour = 0, minute = 0, second = 0, millisecond = 0;
    try {
        if (len >= 4) year = stoi(datetime.substr(0, 4));
        if (len >= 7) month = stoi(datetime.substr(5, 2));
        if (len >= 10) day = stoi(datetime.substr(8, 2));
        if (len >= 13) hour = stoi(datetime.substr(11, 2));
        if (len >= 16) minute = stoi(datetime.substr(14, 2));
        if (len >= 19) second = stoi(datetime.substr(17, 2));
        if (len >= 23) millisecond = stoi(datetime.substr(20, 3));
    } catch (...) {
        return false;
    }

    // Validate ranges (unchanged)
    if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31 ||
        hour < 0 || hour > 23 || minute < 0 || minute > 59 ||
        second < 0 || second > 59 || millisecond < 0 || millisecond > 999) {
        return false;
    }

    // Validate days in months (unchanged)
    if (day > 30 && (month == 4 || month == 6 || month == 9 || month == 11)) return false;
    if (day > 29 && month == 2) return false;
    if (day > 28 && month == 2 && (year % 4 != 0 || (year % 100 == 0 && year % 400 != 0))) return false;

    return true;
}

