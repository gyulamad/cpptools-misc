#pragma once

#include "../TEST.hpp"
#include "../trim.hpp"

#ifdef TEST


TEST(test_trim_no_whitespace) {
    string input = "hello";
    string expected = "hello";
    string actual = trim(input);
    assert(actual == expected && "No whitespace to trim");
}

TEST(test_trim_leading_whitespace) {
    string input = "   hello";
    string expected = "hello";
    string actual = trim(input);
    assert(actual == expected && "Leading whitespace");
}

TEST(test_trim_trailing_whitespace) {
    string input = "hello   ";
    string expected = "hello";
    string actual = trim(input);
    assert(actual == expected && "Trailing whitespace");
}

TEST(test_trim_both_leading_and_trailing_whitespace) {
    string input = "   hello   ";
    string expected = "hello";
    string actual = trim(input);
    assert(actual == expected && "Leading and trailing whitespace");
}

TEST(test_trim_only_whitespace) {
    string input = "   \t\n\r\f\v   ";
    string expected = "";
    string actual = trim(input);
    assert(actual == expected && "Only whitespace");
}

TEST(test_trim_empty_string) {
    string input = "";
    string expected = "";
    string actual = trim(input);
    assert(actual == expected && "Empty string");
}

TEST(test_trim_mixed_whitespace) {
    string input = " \t\nhello\r\f\v ";
    string expected = "hello";
    string actual = trim(input);
    assert(actual == expected && "Mixed whitespace");
}

TEST(test_trim_whitespace_in_middle) {
    string input = "hello world";
    string expected = "hello world";
    string actual = trim(input);
    assert(actual == expected && "Whitespace in middle (should not be trimmed)");
}

#endif
