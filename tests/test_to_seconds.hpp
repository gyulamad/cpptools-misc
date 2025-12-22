#pragma once

#include "../TEST.hpp"
#include "../to_seconds.hpp"

#ifdef TEST


TEST(test_to_seconds_simple) {
    double seconds = to_seconds("1 second");
    assert(seconds == 1.0);
}

#endif