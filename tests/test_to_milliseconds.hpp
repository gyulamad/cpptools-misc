#pragma once

#include "../TEST.hpp"
#include "../to_milliseconds.hpp"

#ifdef TEST


#include "../str_contains.hpp"

TEST(test_to_milliseconds_empty_string) {
    assert(to_milliseconds("") == 0.0);
}

TEST(test_to_milliseconds_single_component_week) {
    double ms = to_milliseconds("1 week");
    assert(ms == 7 * 24 * 3600 * 1000.0);
}

TEST(test_to_milliseconds_multiple_components) {
    double ms = to_milliseconds("1 week 3 days");
    assert(ms == (7 + 3) * 24 * 3600 * 1000.0);
}

TEST(test_to_milliseconds_abbreviations) {
    double ms = to_milliseconds("1Y 2w 3d 4h 5m 6s 7ms");
    double expected = (365 * 24 * 3600 * 1000.0) 
                    + (2 * 7 * 24 * 3600 * 1000.0)
                    + (3 * 24 * 3600 * 1000.0)
                    + (4 * 3600 * 1000.0)
                    + (5 * 60 * 1000.0)
                    + (6 * 1000.0)
                    + 7.0;
    assert(ms == expected);
}

TEST(test_to_milliseconds_case_insensitivity) {
    double ms = to_milliseconds("1 WEEK");
    assert(ms == 7 * 24 * 3600 * 1000.0);
}

TEST(test_to_milliseconds_invalid_number_exception) {
    bool threw = false;
    try {
        to_milliseconds("abc day");
    } catch (const exception& e) {
        assert(str_contains(e.what(), "Invalid number: abc"));
        threw = true;
    }
    assert(threw);
}

TEST(test_to_milliseconds_invalid_unit_exception) {
    bool threw = false;
    try {
        to_milliseconds("1 abc");
    } catch (const exception& e) {
        assert(str_contains(e.what(), "Invalid unit: abc"));
        threw = true;
    }
    assert(threw);
}

TEST(test_to_milliseconds_odd_tokens_exception) {
    bool threw = false;
    try {
        to_milliseconds("1 day 2");
    } catch (const exception& e) {
        assert(str_contains(e.what(), "Input must have pairs of numbers and units"));
        threw = true;
    }
    assert(threw);
}

#endif
