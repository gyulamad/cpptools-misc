#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../capture_cout.hpp"


TEST(test_capture_cout_basic) {
    string actual = capture_cout([]() { cout << "Hello, world!"; });
    assert(actual == "Hello, world!" && "capture_cout: Basic cout capture failed");
}

TEST(test_capture_cout_empty) {
    string actual = capture_cout([]() {});
    assert(actual.empty() && "capture_cout: Empty output should return an empty string");
}

#endif
