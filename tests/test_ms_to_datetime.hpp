#pragma once

#include "../TEST.hpp"
#include "../ms_to_datetime.hpp"

#ifdef TEST

TEST(test_ms_to_datetime) {
    // LCOV_EXCL_START
    // Mock get_time_ms for testing purposes
    auto mock_get_time_ms = []() -> time_ms {
        return 1696516245123LL; // Example timestamp: "2023-10-05 14:30:45.123"
    };
    // LCOV_EXCL_STOP

    // Mock current time
    auto original_get_time_ms = get_time_ms;
    get_time_ms = mock_get_time_ms;

    // Default format with milliseconds (UTC)
    assert(ms_to_datetime(1696516245123LL) == "2023-10-05 14:30:45.123" && "Default format with milliseconds failed");

    // Default format without milliseconds (UTC)
    assert(ms_to_datetime(1696516245123LL, "%Y-%m-%d %H:%M:%S", false) == "2023-10-05 14:30:45" && "Default format without milliseconds failed");

    // Custom format with milliseconds (UTC)
    assert(ms_to_datetime(1696516245123LL, "%Y/%m/%d %H:%M:%S", true) == "2023/10/05 14:30:45.123" && "Custom format with milliseconds failed");

    // Custom format without milliseconds (UTC)
    assert(ms_to_datetime(1696516245123LL, "%Y/%m/%d %H:%M:%S", false) == "2023/10/05 14:30:45" && "Custom format without milliseconds failed");

    // Local time (this depends on the system's timezone, so we only check structure)
    string local_result = ms_to_datetime(1696516245123LL, "%Y-%m-%d %H:%M:%S", true, true);
    assert(local_result.size() == 23 && "Local time format failed");

    // Restore original get_time_ms
    get_time_ms = original_get_time_ms;
}

#endif
