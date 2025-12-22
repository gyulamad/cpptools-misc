#pragma once

#include "../TEST.hpp"
#include "../datetime_to_ms.hpp"

#ifdef TEST


TEST(test_datetime_to_ms) {
    time_ms actual;

    // Full datetime string
    assert(datetime_to_ms("2023-10-05 14:30:45.123") == 1696516245123LL && "Full datetime test failed");

    // Missing milliseconds
    assert(datetime_to_ms("2023-10-05 14:30:45") == 1696516245000LL && "Missing milliseconds test failed");

    // Missing seconds
    assert(datetime_to_ms("2023-10-05 14:30") == 1696516200000LL && "Missing seconds test failed");

    // Missing minutes
    actual = datetime_to_ms("2023-10-05 14");
    assert(actual == 1696514400000LL && "Missing minutes test failed");

    // Missing hours
    assert(datetime_to_ms("2023-10-05") == 1696464000000LL && "Missing hours test failed");

    // Missing day
    assert(datetime_to_ms("2023-10") == 1696118400000LL && "Missing day test failed");

    // Missing month
    actual = datetime_to_ms("2023");
    assert(actual == 1672531200000LL && "Missing month test failed");

    // Empty string
    bool thrown = false;
    try {
        datetime_to_ms("");
    } catch (...) {
        thrown = true;
    }
    assert(thrown && "Empty string test failed");

    // Invalid input (should default to epoch)
    thrown = false;
    try {
        datetime_to_ms("invalid-date");
    } catch (...) {
        thrown = true;
    }
    assert(thrown && "Invalid input test failed");
}

#endif
