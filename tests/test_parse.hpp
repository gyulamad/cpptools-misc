#pragma once

#include "../TEST.hpp"
#include "../parse.hpp"

#ifdef TEST

#include <cmath>

TEST(test_parse_valid_integer) {
    int expected = 42;
    int actual = parse<int>("42");
    assert(actual == expected && "test_parse_valid_integer failed");
}

TEST(test_parse_valid_double) {
    double expected = 3.14;
    double actual = parse<double>("3.14");
    assert(actual == expected && "test_parse_valid_double failed");
}

TEST(test_parse_negative_number) {
    int expected = -10;
    int actual = parse<int>("-10");
    assert(actual == expected && "test_parse_negative_number failed");
}

TEST(test_parse_invalid_input) {
    bool thrown = false;
    try {
        parse<int>("abc");
    } catch (const exception&) {
        thrown = true;
    }
    assert(thrown && "test_parse_invalid_input failed");
}

TEST(test_parse_empty_string) {
    bool thrown = false;
    try {
        parse<double>("");
    } catch (const exception&) {
        thrown = true;
    }
    assert(thrown && "test_parse_empty_string failed");
}

TEST(test_parse_whitespace_only) {
    bool thrown = false;
    try {
        parse<int>("   ");
    } catch (const exception&) {
        thrown = true;
    }
    assert(thrown && "test_parse_whitespace_only failed");
}

TEST(test_parse_trailing_characters) {    
    int actual = parse<int>("42abc");
    assert(actual == 42 && "test_parse_trailing_characters failed");
}

TEST(test_parse_trailing_characters_negative) {    
    int actual = parse<int>("-42abc");
    assert(actual == -42 && "test_parse_trailing_characters_negative failed");
}

TEST(test_parse_floating_point_with_exponent) {
    double expected = 1.23e3;
    double actual = parse<double>("1.23e3");
    assert(actual == expected && "test_parse_floating_point_with_exponent failed");
}

TEST(test_parse_zero) {
    int expected = 0;
    int actual = parse<int>("0");
    assert(actual == expected && "test_parse_zero failed");
}

TEST(test_parse_large_number) {
    long long expected = 9223372036854775807LL;
    long long actual = parse<long long>("9223372036854775807");
    assert(actual == expected && "test_parse_large_number failed");
}

TEST(test_parse_positive_infinity_double) {
    double actual = parse<double>("inf");
    assert(isinf(actual) && actual > 0 && "test_parse_positive_infinity_double failed");
}

TEST(test_parse_positive_infinity_explicit_double) {
    double actual = parse<double>("+inf");
    assert(isinf(actual) && actual > 0 && "test_parse_positive_infinity_explicit_double failed");
}

TEST(test_parse_negative_infinity_double) {
    double actual = parse<double>("-inf");
    assert(isinf(actual) && actual < 0 && "test_parse_negative_infinity_double failed");
}

TEST(test_parse_nan_double) {
    double actual = parse<double>("nan");
    assert(isnan(actual) && "test_parse_nan_double failed");
}

TEST(test_parse_positive_infinity_float) {
    float actual = parse<float>("inf");
    assert(isinf(actual) && actual > 0 && "test_parse_positive_infinity_float failed");
}

TEST(test_parse_positive_infinity_explicit_float) {
    float actual = parse<float>("+inf");
    assert(isinf(actual) && actual > 0 && "test_parse_positive_infinity_explicit_float failed");
}

TEST(test_parse_negative_infinity_float) {
    float actual = parse<float>("-inf");
    assert(isinf(actual) && actual < 0 && "test_parse_negative_infinity_float failed");
}

TEST(test_parse_nan_float) {
    float actual = parse<float>("nan");
    assert(isnan(actual) && "test_parse_nan_float failed");
}

TEST(test_parse_infinity_invalid_int) {
    bool thrown = false;
    try {
        parse<int>("inf");
    } catch (const exception&) {
        thrown = true;
    }
    assert(thrown && "test_parse_infinity_invalid_int failed");
}

TEST(test_parse_nan_invalid_int) {
    bool thrown = false;
    try {
        parse<int>("nan");
    } catch (const exception&) {
        thrown = true;
    }
    assert(thrown && "test_parse_nan_invalid_int failed");
}

#endif
