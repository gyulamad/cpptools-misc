#pragma once

#include "../TEST.hpp"
#include "../is_valid_datetime.hpp"

#ifdef TEST


TEST(test_is_valid_datetime) {
    // Valid datetimes
    assert(is_valid_datetime("2023") == true); // Year only
    assert(is_valid_datetime("2023-01") == true); // Year and month
    assert(is_valid_datetime("2023-01-01") == true); // Year, month, and day
    assert(is_valid_datetime("2023-01-01 12") == true); // Year, month, day, and hour
    assert(is_valid_datetime("2023-01-01 12:34") == true); // Year, month, day, hour, and minute
    assert(is_valid_datetime("2023-01-01 12:34:45") == true); // Year, month, day, hour, minute, and second
    assert(is_valid_datetime("2023-01-01 12:34:45.123") == true); // Full datetime

    // Invalid datetimes
    assert(is_valid_datetime("") == false); // Empty string
    assert(is_valid_datetime("2023-13") == false); // Invalid month
    assert(is_valid_datetime("2023-01-32") == false); // Invalid day
    assert(is_valid_datetime("2023-02-29") == false); // Non-leap year
    assert(is_valid_datetime("2023-01-01 24") == false); // Invalid hour
    assert(is_valid_datetime("2023-01-01 12:60") == false); // Invalid minute
    assert(is_valid_datetime("2023-01-01 12:34:60") == false); // Invalid second
    assert(is_valid_datetime("2023-01-01 12:34:45.") == false); // Incomplete milliseconds

    // Invalid format
    assert(is_valid_datetime("2023/01/01") == false); // Wrong separator
    assert(is_valid_datetime("2023-01-01T12:34:45") == false); // Wrong separator
    assert(is_valid_datetime("2023-01-01 12-34-45") == false); // Wrong separator
}

#endif
