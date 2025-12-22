#pragma once

#include "../TEST.hpp"
#include "../date_to_sec.hpp"

#ifdef TEST


TEST(test_date_to_sec) {
    // Full date string
    assert(date_to_sec("2023-10-05") == 1696464000 && "Full date test failed");

    // Missing day
    assert(date_to_sec("2023-10") == 1696118400 && "Missing day test failed");

    // Missing month
    time_t actual = date_to_sec("2023");
    assert(actual == 1672531200 && "Missing month test failed");

    // Empty string
    bool thrown = false;
    try {
        date_to_sec("");
    } catch (...) {
        thrown = true;
    }
    assert(thrown && "Empty string test failed");

    // Invalid input (should default to epoch)
    thrown = false;
    try {
        date_to_sec("invalid-date");
    } catch (...) {
        thrown = true;
    }
    assert(thrown && "Invalid input test failed");
}

#endif
