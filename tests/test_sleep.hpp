#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../sleep.hpp"
#include "../Stopper.hpp"
#include "../ERROR.hpp"

TEST(test_sleep_s_1) {
    Stopper stopper(false);
    stopper.start();
    sleep_s(1);
    double elapsed = stopper.stop();
    // Allow 10% tolerance (900ms - 1100ms for 1 second sleep)
    assert(elapsed >= 900 && elapsed <= 1100);
}

TEST(test_sleep_s_2) {
    Stopper stopper(false);
    stopper.start();
    sleep_s(2);
    double elapsed = stopper.stop();
    // Allow 10% tolerance (1900ms - 2100ms for 2 second sleep)
    assert(elapsed >= 1900 && elapsed <= 2100);
}

TEST(test_sleep_ms_500) {
    Stopper stopper(false);
    stopper.start();
    sleep_ms(500);
    double elapsed = stopper.stop();
    // Allow 10% tolerance (450ms - 550ms for 500ms sleep)
    assert(elapsed >= 450 && elapsed <= 550);
}

TEST(test_sleep_ms_1000) {
    Stopper stopper(false);
    stopper.start();
    sleep_ms(1000);
    double elapsed = stopper.stop();
    // Allow 10% tolerance (900ms - 1100ms for 1000ms sleep)
    assert(elapsed >= 900 && elapsed <= 1100);
}

TEST(test_sleep_ms_250) {
    Stopper stopper(false);
    stopper.start();
    sleep_ms(250);
    double elapsed = stopper.stop();
    // Allow 15% tolerance (212ms - 287ms for 250ms sleep)
    assert(elapsed >= 212 && elapsed <= 287);
}

#endif