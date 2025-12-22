#pragma once

#include "../TEST.hpp"
#include "../date_to_ms.hpp"

#ifdef TEST


TEST(test_date_to_ms) {
    // Full date string
    assert(date_to_ms("2023-10-05") == 1696464000000LL && "Full date test failed");

    // Missing day
    assert(date_to_ms("2023-10") == 1696118400000LL && "Missing day test failed");

    // Missing month
    time_t actual = date_to_ms("2023");
    assert(actual == 1672531200000LL && "Missing month test failed");


    // Empty string
    bool thrown = false;
    try {
        date_to_ms("");
    } catch (...) {
        thrown = true;
    }
    assert(thrown && "Empty string test failed");

    // Invalid input (should default to epoch)
    thrown = false;
    try {
        date_to_ms("invalid-date");
    } catch (...) {
        thrown = true;
    }
    assert(thrown && "Invalid input test failed");
}

#endif
