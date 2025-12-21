#pragma once

#include "../TEST.hpp"
#include "../capture_cout_cerr.hpp"

#ifdef TEST

TEST(test_capture_cout_cerr_basic) {
    string actual = capture_cout_cerr([]() {
        cout << "Stdout message. ";
        cerr << "Stderr message.";
    });
    assert(actual == "Stdout message. Stderr message." && "capture_cout_cerr: Combined output capture failed");
}

TEST(test_capture_cout_cerr_empty) {
    string actual = capture_cout_cerr([]() {});
    assert(actual.empty() && "capture_cout_cerr: Empty output should return an empty string");
}

#endif
