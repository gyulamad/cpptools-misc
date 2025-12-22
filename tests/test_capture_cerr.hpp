#pragma once

#include "../TEST.hpp"
#include "../capture_cerr.hpp"

#ifdef TEST


TEST(test_capture_cerr_basic) {
    string actual = capture_cerr([]() { cerr << "Error occurred!"; });
    assert(actual == "Error occurred!" && "capture_cerr: Basic cerr capture failed");
}

TEST(test_capture_cerr_empty) {
    string actual = capture_cerr([]() {});
    assert(actual.empty() && "capture_cerr: Empty output should return an empty string");
}

#endif
