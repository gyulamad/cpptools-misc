#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include <thread>
#include <chrono>

using namespace std;

// Test that completes quickly
TEST(test_timeout_quick_test) {
    // This test should complete quickly
    assert(true);
}

// Test that uses TEST_TIMEOUT constant
TEST(test_timeout_constant) {
    // Just verify the constant is defined
    #ifdef TEST_TIMEOUT
        assert(TEST_TIMEOUT > 0 && "TEST_TIMEOUT should be defined and positive");
    #else
        assert(false && "TEST_TIMEOUT should be defined");
    #endif
}

#endif // TEST