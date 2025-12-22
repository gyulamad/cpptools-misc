#pragma once

#include "../TEST.hpp"
#include "../sec_to_datetime.hpp"

#ifdef TEST


TEST(test_sec_to_datetime) {
    // Default format (UTC)
    assert(sec_to_datetime(1696516245) == "2023-10-05 14:30:45" && "Default format failed");

    // Custom format (UTC)
    assert(sec_to_datetime(1696516245, "%Y/%m/%d %H:%M:%S") == "2023/10/05 14:30:45" && "Custom format failed");

    // Local time (structure check)
    string local_result = sec_to_datetime(1696516245, "%Y-%m-%d %H:%M:%S", true);
    assert(local_result.size() == 19 && "Local time format failed");
}

#endif
